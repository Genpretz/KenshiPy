#include "PyRuntime.h"
#include "Logger.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <fstream>
#include <functional>
#include <string>
#include <vector>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <rapidjson/istreamwrapper.h>

#include <kenshi/GameWorld.h>
#include <kenshi/Globals.h>
#include <kenshi/ModInfo.h>

// Forward declaration of the SWIG-generated module init function.
extern "C" PyMODINIT_FUNC PyInit__KenshiPy(void);

// ---------------------------------------------------------------------------
// Output sink
// ---------------------------------------------------------------------------

static PythonOutputSink g_outputSink;   // empty == use Logger

static void SendToSink(const std::string& text)
{
    if (g_outputSink)
        g_outputSink(text);
    else
        Logger::Debug(text);
}

void SetPythonOutputSink(PythonOutputSink sink)
{
    g_outputSink = std::move(sink);
}

// ---------------------------------------------------------------------------
// Python stdout/stderr redirector type
//
// Installed once in InitPython().  Every print() or traceback goes through
// SendToSink(), which routes to whoever called SetPythonOutputSink() most
// recently (ScriptEditor while the editor is open, Logger otherwise).
// ---------------------------------------------------------------------------

static PyObject* Redirector_write(PyObject* /*self*/, PyObject* args)
{
    const char* text = nullptr;
    if (!PyArg_ParseTuple(args, "s", &text))
        Py_RETURN_NONE;

    if (text && text[0] != '\0')
        SendToSink(text);

    Py_RETURN_NONE;
}

static PyObject* Redirector_flush(PyObject* /*self*/, PyObject* /*args*/)
{
    Py_RETURN_NONE;
}

static PyMethodDef g_redirectorMethods[] =
{
    { "write", Redirector_write, METH_VARARGS, "write(text)" },
    { "flush", Redirector_flush, METH_VARARGS, "flush()"     },
    { nullptr, nullptr, 0, nullptr }
};

static void InstallOutputRedirect()
{
    // Build a minimal module-like object and assign it to sys.stdout/stderr.
    PyObject* mod = PyModule_New("_kenshipy_redirect");
    if (!mod)
        return;

    PyObject* writeFunc = PyCFunction_New(&g_redirectorMethods[0], nullptr);
    PyObject* flushFunc = PyCFunction_New(&g_redirectorMethods[1], nullptr);

    if (writeFunc) PyModule_AddObject(mod, "write", writeFunc);
    if (flushFunc) PyModule_AddObject(mod, "flush", flushFunc);

    PyObject* sys = PyImport_ImportModule("sys");
    if (sys)
    {
        PyObject_SetAttrString(sys, "stdout", mod);
        PyObject_SetAttrString(sys, "stderr", mod);
        Py_DECREF(sys);
    }

    Py_DECREF(mod);
}

// ---------------------------------------------------------------------------
// Error formatting
// ---------------------------------------------------------------------------

// Fetch and format the current Python exception (including traceback) into
// a readable string, then clear the exception state.
static std::string FetchPythonError(const std::string& context)
{
    PyObject* ptype     = nullptr;
    PyObject* pvalue    = nullptr;
    PyObject* ptraceback = nullptr;
    PyErr_Fetch(&ptype, &pvalue, &ptraceback);
    PyErr_NormalizeException(&ptype, &pvalue, &ptraceback);

    std::string msg = context + "\n";

    // Try to use the traceback module for a full formatted traceback.
    PyObject* tbMod = PyImport_ImportModule("traceback");
    if (tbMod)
    {
        PyObject* fmt = PyObject_GetAttrString(tbMod, "format_exception");
        if (fmt)
        {
            PyObject* parts = PyObject_CallFunctionObjArgs(
                fmt,
                ptype      ? ptype      : Py_None,
                pvalue     ? pvalue     : Py_None,
                ptraceback ? ptraceback : Py_None,
                nullptr);

            if (parts)
            {
                PyObject* sep    = PyUnicode_FromString("");
                PyObject* joined = sep ? PyUnicode_Join(sep, parts) : nullptr;
                if (joined)
                {
                    const char* text = PyUnicode_AsUTF8(joined);
                    if (text)
                        msg += text;
                    Py_DECREF(joined);
                }
                Py_XDECREF(sep);
                Py_DECREF(parts);
            }
            Py_DECREF(fmt);
        }
        Py_DECREF(tbMod);
    }

    // Fallback: just stringify the value.
    if (msg == context + "\n" && pvalue)
    {
        PyObject* s = PyObject_Str(pvalue);
        if (s)
        {
            const char* text = PyUnicode_AsUTF8(s);
            if (text)
                msg += text;
            Py_DECREF(s);
        }
    }

    Py_XDECREF(ptype);
    Py_XDECREF(pvalue);
    Py_XDECREF(ptraceback);

    if (PyErr_Occurred())
        PyErr_Clear();

    return msg;
}

// ---------------------------------------------------------------------------
// Path helpers
// ---------------------------------------------------------------------------

static std::string GetExeDirectory()
{
    char path[MAX_PATH];
    GetModuleFileNameA(nullptr, path, MAX_PATH);
    std::string s(path);
    size_t pos = s.find_last_of("\\/");
    return (pos != std::string::npos) ? s.substr(0, pos) : ".";
}

static std::string GetDllDirectory()
{
    char path[MAX_PATH];
    GetModuleFileNameA(GetModuleHandleA("KenshiPy.dll"), path, MAX_PATH);
    std::string s(path);
    size_t pos = s.find_last_of("\\/");
    return (pos != std::string::npos) ? s.substr(0, pos) : ".";
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

void InitPython()
{
    // Register our SWIG module before Py_Initialize so it can be imported
    // as "_KenshiPy" without a .pyd on disk.
    PyImport_AppendInittab("_KenshiPy", PyInit__KenshiPy);

    Py_Initialize();
    PyEval_InitThreads();   // required for GIL-based multi-threading

    // Extend sys.path so scripts can import from the mod folder and the
    // Kenshi executable directory.
    PyObject* sysPath = PySys_GetObject("path");   // borrowed ref

    auto prependPath = [&](const std::string& dir)
    {
        PyObject* p = PyUnicode_FromString(dir.c_str());
        PyList_Insert(sysPath, 0, p);
        Py_DECREF(p);
    };

    prependPath(GetExeDirectory());
    prependPath(GetDllDirectory());

    // Redirect stdout/stderr — do this once here, not in every UI panel.
    InstallOutputRedirect();

    // Release the GIL so other threads can use Python.
    PyEval_SaveThread();

    Logger::Debug("Python interpreter initialised.");
}

void ShutdownPython()
{
    if (!Py_IsInitialized())
        return;

    PyGILState_STATE gs = PyGILState_Ensure();
    (void)gs;
    Py_Finalize();
}

bool ExecutePython(const std::string& code,
                   const std::string& filename,
                   int                startToken)
{
    PyGILState_STATE gs = PyGILState_Ensure();

    PyObject* codeObj = Py_CompileString(code.c_str(), filename.c_str(), startToken);
    if (!codeObj)
    {
        std::string err = FetchPythonError("Compile error in " + filename);
        SendToSink(err);
        Logger::Error(err);
        PyGILState_Release(gs);
        return false;
    }

    PyObject* mainMod = PyImport_AddModule("__main__");   // borrowed
    PyObject* globals  = PyModule_GetDict(mainMod);        // borrowed
    PyObject* result   = PyEval_EvalCode(codeObj, globals, globals);

    bool ok = (result != nullptr);
    if (!ok)
    {
        std::string err = FetchPythonError("Runtime error in " + filename);
        SendToSink(err);
        Logger::Error(err);
    }

    Py_XDECREF(result);
    Py_DECREF(codeObj);
    PyGILState_Release(gs);
    return ok;
}

void RunScript(const std::string& scriptPath)
{
    std::ifstream file(scriptPath.c_str(), std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        Logger::Error("Could not open script: " + scriptPath);
        return;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buf(static_cast<size_t>(size) + 1, '\0');
    if (!file.read(buf.data(), size))
    {
        Logger::Error("Could not read script: " + scriptPath);
        return;
    }

    // Push the script filename as a logger context so every log line during
    // execution is prefixed with [filename].
    Logger::ScopedContext ctx(scriptPath);
    ExecutePython(buf.data(), scriptPath, Py_file_input);
}

void RunString(const std::string& code)
{
    ExecutePython(code, "<string>", Py_file_input);
}

// ---------------------------------------------------------------------------
// Mod loading
// ---------------------------------------------------------------------------

static bool g_modsLoaded = false;

static void LoadModScripts(lektor<ModInfo*>& mods)
{
    for (int i = 0; i < mods.size(); ++i)
    {
        const std::string jsonPath = mods[i]->path + "\\KenshiPy.json";
        std::ifstream jsonFile(jsonPath.c_str());

        if (!jsonFile.is_open())
            continue;  // this mod has no KenshiPy scripts — that's fine

        rapidjson::IStreamWrapper isw(jsonFile);
        rapidjson::Document dom;
        if (dom.ParseStream(isw).HasParseError())
        {
            Logger::Error("JSON parse error in \"" + jsonPath + "\": "
                          + rapidjson::GetParseError_En(dom.GetParseError()));
            continue;
        }

        if (!dom.HasMember("Scripts") || !dom["Scripts"].IsArray())
        {
            Logger::Error("\"Scripts\" array missing or wrong type in \"" + jsonPath + "\"");
            continue;
        }

        const rapidjson::Value& scripts = dom["Scripts"];
        for (auto itr = scripts.Begin(); itr != scripts.End(); ++itr)
        {
            if (!itr->IsString())
            {
                Logger::Error("Non-string entry in Scripts array in \"" + jsonPath + "\"");
                continue;
            }

            const std::string scriptPath = mods[i]->path + "\\" + itr->GetString();
            Logger::Debug("Loading " + mods[i]->name + " -> " + itr->GetString());
            RunScript(scriptPath);
        }
    }
}

void TryLoadMods()
{
    if (g_modsLoaded)
        return;

    if (!ou)
    {
        Logger::Debug("GameWorld not ready yet — deferring mod script loading.");
        return;
    }

    if (ou->activeMods.size() == 0)
    {
        Logger::Debug("No active mods.");
        return;
    }

    g_modsLoaded = true;
    Logger::Debug("Loading mod scripts...");
    LoadModScripts(ou->activeMods);
}
