#pragma once

#include <string>
#include <Python.h>

void InitPython();
void ShutdownPython();
void TryLoadMods();

__declspec(dllexport) void RunScript(const std::string& scriptPath);
__declspec(dllexport) void RunString(const std::string& code);

void RegisterKeyDownCallback(PyObject* callable);
void CallKeyDownCallbacks(int keyCode);