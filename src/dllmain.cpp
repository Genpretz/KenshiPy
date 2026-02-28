#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <Python.h>

#include "dllmain.h"
#include "KenshiPy_Plugin.h"

#include <Debug.h>
#include <core/Functions.h>
#include <kenshi/Kenshi.h>
#include <kenshi/GameWorld.h>
#include <kenshi/ModInfo.h>
#include <kenshi/Globals.h>


#include <rapidjson/istreamwrapper.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#include <string>
#include <fstream>
#include <vector>

KenshiPy_Plugin* KPyPlugin;

// Forward declaration of the KenshiPy SWIG module init function
extern "C" PyMODINIT_FUNC PyInit__KenshiPy(void);

// ----------------------------------------------------------------------------
// Helpers
// ----------------------------------------------------------------------------

static std::string GetKenshiDir()
{
    CHAR path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    std::string full(path);
    size_t pos = full.find_last_of("\\/");
    if (pos != std::string::npos)
        return full.substr(0, pos);
    return ".";
}

static void RunScript(const std::string& scriptPath)
{
    std::ifstream file(scriptPath.c_str(), std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        ErrorLog("KenshiPy: Could not open script: " + scriptPath);
        return;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(static_cast<size_t>(size) + 1, 0);
    if (!file.read(buffer.data(), size))
    {
        ErrorLog("KenshiPy: Could not read script: " + scriptPath);
        return;
    }

    PyGILState_STATE gstate = PyGILState_Ensure();

    PyObject* code = Py_CompileString(buffer.data(), scriptPath.c_str(), Py_file_input);
    if (!code)
    {
        PyErr_Print();
        ErrorLog("KenshiPy: Compile error in script: " + scriptPath);
        PyGILState_Release(gstate);
        return;
    }

    PyObject* mainModule = PyImport_AddModule("__main__");
    PyObject* globals    = PyModule_GetDict(mainModule);
    PyObject* result     = PyEval_EvalCode(code, globals, globals);

    if (!result)
        PyErr_Print();

    Py_XDECREF(result);
    Py_DECREF(code);

    PyGILState_Release(gstate);
}

// ----------------------------------------------------------------------------
// Script loading - iterates active mods looking for KenshiPy.json
// ----------------------------------------------------------------------------

static void LoadModScripts(lektor<ModInfo*>& mods)
{
    for (int i = 0; i < mods.size(); ++i)
    {
        std::string jsonPath = mods[i]->path + "\\KenshiPy.json";
        std::ifstream jsonFile(jsonPath.c_str());

        if (!jsonFile.is_open())
            continue;

        rapidjson::IStreamWrapper isw(jsonFile);
        rapidjson::Document dom;
        if (dom.ParseStream(isw).HasParseError())
        {
            ErrorLog("KenshiPy: Error parsing \"" + jsonPath + "\": "
                + rapidjson::GetParseError_En(dom.GetParseError()));
            continue;
        }

        if (!dom.HasMember("Scripts") || !dom["Scripts"].IsArray())
            continue;

        const rapidjson::Value& scripts = dom["Scripts"];
        for (rapidjson::Value::ConstValueIterator itr = scripts.Begin();
             itr != scripts.End(); ++itr)
        {
            if (!itr->IsString())
                continue;

            std::string scriptPath = mods[i]->path + "\\" + itr->GetString();
            DebugLog("KenshiPy: Loading " + mods[i]->name + " -> " + itr->GetString());
            RunScript(scriptPath);
        }
    }
}

// ----------------------------------------------------------------------------
// initModsList hook - fires after mod list is ready (preload equivalent)
// ----------------------------------------------------------------------------

static bool hasLoaded = false;

void (*initModsList_orig)(GameWorld*) = NULL;
void initModsList_hook(GameWorld* thisptr)
{
    initModsList_orig(thisptr);

    if (!hasLoaded)
    {
        hasLoaded = true;
        DebugLog("KenshiPy: Loading mod scripts...");
        LoadModScripts(ou->availabelModsOrderedList);
    }
}

// ----------------------------------------------------------------------------
// Python interpreter lifetime
// ----------------------------------------------------------------------------

static void InitPython()
{
    // Register KenshiPy module before Py_Initialize so it is importable
    PyImport_AppendInittab("_KenshiPy", PyInit__KenshiPy);

    Py_Initialize();
    PyEval_InitThreads();

    // Set sys.path to include Kenshi root so scripts can import local modules
    std::string kenshiDir = GetKenshiDir();
    PyObject* sysPath = PySys_GetObject("path");
    PyObject* pathStr = PyUnicode_FromString(kenshiDir.c_str());
    PyList_Insert(sysPath, 0, pathStr);
    Py_DECREF(pathStr);

    // Release GIL - game threads will acquire it as needed via PyGILState_Ensure
    PyEval_SaveThread();

    DebugLog("KenshiPy: Python interpreter initialized.");
}

static void ShutdownPython()
{
    PyGILState_STATE gstate = PyGILState_Ensure();
    Py_Finalize();
}

// ----------------------------------------------------------------------------
// Plugin entry points
// ----------------------------------------------------------------------------

static void Init()
{
    InitPython();
    KenshiLib::AddHook(
        KenshiLib::GetRealAddress(&GameWorld::initModsList),
        initModsList_hook,
        &initModsList_orig);
    DebugLog("KenshiPy: Runtime initialized.");
}

// Ogre plugin system exports
extern "C" __declspec(dllexport) void dllStartPlugin(void)
{
    // Create plugin
    KPyPlugin = new KenshiPy_Plugin();

    // Register
    Ogre::Root::getSingleton().installPlugin(KPyPlugin);
}

extern "C" __declspec(dllexport) void dllStopPlugin(void)
{

    Ogre::Root::getSingleton().uninstallPlugin(KPyPlugin);
    delete KPyPlugin;
    ShutdownPython();
}

// RE_Kenshi plugin export (matches the mangled name RE_Kenshi expects)
__declspec(dllexport) void startPlugin()
{
    Init();
}

// ----------------------------------------------------------------------------
// DLL entry point
// ----------------------------------------------------------------------------

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    return TRUE;
}
