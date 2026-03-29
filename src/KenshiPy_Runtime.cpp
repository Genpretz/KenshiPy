#include "KenshiPy_Runtime.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <fstream>
#include <string>
#include <vector>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <rapidjson/istreamwrapper.h>

#include <Debug.h>
#include <core/Functions.h>
#include <kenshi/GameWorld.h>
#include <kenshi/Globals.h>
#include <kenshi/ModInfo.h>

// Forward declaration of the KenshiPy SWIG module init function
extern "C" PyMODINIT_FUNC PyInit__KenshiPy(void);

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

void RunScript(const std::string& scriptPath)
{
	std::ifstream file(scriptPath.c_str(), std::ios::binary | std::ios::ate);
	if (!file.is_open())
	{
		ErrorLog("Could not open script: " + scriptPath);
		return;
	}

	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<char> buffer(static_cast<size_t>(size) + 1, 0);
	if (!file.read(buffer.data(), size))
	{
		ErrorLog("Could not read script: " + scriptPath);
		return;
	}

	PyGILState_STATE gstate = PyGILState_Ensure();

	PyObject* code = Py_CompileString(buffer.data(), scriptPath.c_str(), Py_file_input);
	if (!code)
	{
		PyObject* ptype, * pvalue, * ptraceback;
		PyErr_Fetch(&ptype, &pvalue, &ptraceback);
		PyErr_NormalizeException(&ptype, &pvalue, &ptraceback);

		std::string errorMsg = "Compile error in: " + scriptPath + "\n";
		if (pvalue)
		{
			PyObject* pstr = PyObject_Str(pvalue);
			if (pstr)
			{
				errorMsg += PyUnicode_AsUTF8(pstr);
				Py_DECREF(pstr);
			}
		}

		ErrorLog(errorMsg);
		//OutputDebugStringA(errorMsg.c_str());

		Py_XDECREF(ptype);
		Py_XDECREF(pvalue);
		Py_XDECREF(ptraceback);
		PyGILState_Release(gstate);
		return;
	}

	PyObject* mainModule = PyImport_AddModule("__main__");
	PyObject* globals = PyModule_GetDict(mainModule);
	PyObject* result = PyEval_EvalCode(code, globals, globals);

	if (!result)
	{
		PyObject* ptype, * pvalue, * ptraceback;
		PyErr_Fetch(&ptype, &pvalue, &ptraceback);
		PyErr_NormalizeException(&ptype, &pvalue, &ptraceback);

		std::string errorMsg = "KenshiPy: Runtime error in: " + scriptPath + "\n";
		if (pvalue)
		{
			PyObject* pstr = PyObject_Str(pvalue);
			if (pstr)
			{
				errorMsg += PyUnicode_AsUTF8(pstr);
				Py_DECREF(pstr);
			}
		}

		ErrorLog(errorMsg);
		//OutputDebugStringA(errorMsg.c_str());

		Py_XDECREF(ptype);
		Py_XDECREF(pvalue);
		Py_XDECREF(ptraceback);
	}

	Py_XDECREF(result);
	Py_DECREF(code);

	PyGILState_Release(gstate);
}

void RunString(const std::string& code)
{
	PyGILState_STATE gstate = PyGILState_Ensure();

	PyObject* code_obj = Py_CompileString(code.c_str(), "<string>", Py_file_input);
	if (!code_obj)
	{
		PyObject* ptype, * pvalue, * ptraceback;
		PyErr_Fetch(&ptype, &pvalue, &ptraceback);
		PyErr_NormalizeException(&ptype, &pvalue, &ptraceback);

		std::string errorMsg = "Compile error in RunString\n";
		if (pvalue)
		{
			PyObject* pstr = PyObject_Str(pvalue);
			if (pstr)
			{
				errorMsg += PyUnicode_AsUTF8(pstr);
				Py_DECREF(pstr);
			}
		}

		ErrorLog(errorMsg);
		//OutputDebugStringA(errorMsg.c_str());

		Py_XDECREF(ptype);
		Py_XDECREF(pvalue);
		Py_XDECREF(ptraceback);
		PyGILState_Release(gstate);
		return;
	}

	PyObject* mainModule = PyImport_AddModule("__main__");
	PyObject* globals = PyModule_GetDict(mainModule);
	PyObject* result = PyEval_EvalCode(code_obj, globals, globals);

	if (!result)
	{
		PyObject* ptype, * pvalue, * ptraceback;
		PyErr_Fetch(&ptype, &pvalue, &ptraceback);
		PyErr_NormalizeException(&ptype, &pvalue, &ptraceback);

		std::string errorMsg = "KenshiPy: Runtime error in RunString\n";
		if (pvalue)
		{
			PyObject* pstr = PyObject_Str(pvalue);
			if (pstr)
			{
				errorMsg += PyUnicode_AsUTF8(pstr);
				Py_DECREF(pstr);
			}
		}

		ErrorLog(errorMsg);
		//OutputDebugStringA(errorMsg.c_str());

		Py_XDECREF(ptype);
		Py_XDECREF(pvalue);
		Py_XDECREF(ptraceback);
	}

	Py_XDECREF(result);
	Py_DECREF(code_obj);

	PyGILState_Release(gstate);
}

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
			ErrorLog("Error parsing \"" + jsonPath + "\": "
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
			DebugLog("Loading " + mods[i]->name + " -> " + itr->GetString());
			RunScript(scriptPath);
		}
	}
}

static bool g_loaded = false;

void TryLoadMods()
{
	if (g_loaded)
		return;

	// global variable GameWorld* ou is defined in Globals.h
	if (ou->activeMods.size() == 0)
	{
		DebugLog("No active mods. Skipping script loading.");
		return;
	}

	g_loaded = true;
	DebugLog("Loading active mod scripts...");
	LoadModScripts(ou->activeMods);
}

void InitPython()
{
	// Register KenshiPy module before Py_Initialize so it is importable
	PyImport_AppendInittab("_KenshiPy", PyInit__KenshiPy);

	Py_Initialize();
	PyEval_InitThreads();

	// Add both Kenshi root and KenshiPython\ to sys.path.
	// KenshiPython\ is where KenshiPy.py lives so 'import KenshiPy' works.
	// Kenshi root is kept so scripts can import local modules from the game dir.
	std::string kenshiDir = GetKenshiDir();
	std::string kenshiPyDir = kenshiDir + "\\KenshiPython";

	PyObject* sysPath = PySys_GetObject("path");

	PyObject* kenshiPyPath = PyUnicode_FromString(kenshiPyDir.c_str());
	PyList_Insert(sysPath, 0, kenshiPyPath);
	Py_DECREF(kenshiPyPath);

	PyObject* kenshiPath = PyUnicode_FromString(kenshiDir.c_str());
	PyList_Insert(sysPath, 0, kenshiPath);
	Py_DECREF(kenshiPath);

	// Release GIL, game threads will acquire it as needed via PyGILState_Ensure
	PyEval_SaveThread();

	DebugLog("Python interpreter initialized.");
}

void ShutdownPython()
{
	PyGILState_STATE gstate = PyGILState_Ensure();
	Py_Finalize();
}