#pragma once

#include <string>

void InitPython();
void ShutdownPython();
void TryLoadMods();

__declspec(dllexport) void RunScript(const std::string& scriptPath);
__declspec(dllexport) void RunString(const std::string& code);