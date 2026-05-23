#include "Hooks.h"
#include "PyRuntime.h"
#include "Logger.h"
#include "ScriptEditor.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <core/Functions.h>
#include "mygui/MyGUI_Gui.h"
#include "kenshi/Kenshi.h"

// ----------------------------------------------------------------------------
// RE_Kenshi Plugin Entry Point
// ----------------------------------------------------------------------------

__declspec(dllexport) void startPlugin()
  {
    InitLogger("mods\\KenshiPython\\KenshiPy.log");

    Logger::Debug("KenshiPy startPlugin called.");

    HMODULE kenshiLib = GetModuleHandleA("KenshiLib-py.dll");
    Logger::Debug("KenshiLib-py.dll handle: 0x%p", kenshiLib);
    if (kenshiLib == nullptr)
    {
        Logger::Error("KenshiLib-py.dll not found. Plugin cannot function.");
        return;
    }

    if (KenshiLib::Init())
    {
        Logger::Debug("KenshiLib-py initialized successfully.");
        if (!InstallHooks())
        {
            return;
        }
    }
    else
    {
        Logger::Error("Failed to initialize KenshiLib-py. Plugin cannot function.");
        return;
    }
    std::string test = Logger::GetLog();

    InitPython();
    Logger::Debug("KenshiPy initialization complete.");
    TryLoadMods();
}

// ----------------------------------------------------------------------------
// DLL Entry Point
// ----------------------------------------------------------------------------

//BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpvReserved)
//{
//    return TRUE;
//}
