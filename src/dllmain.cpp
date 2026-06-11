#include "Hooks.h"
#include "PyRuntime.h"
#include "Logger.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include "kenshi/Kenshi.h"

// ----------------------------------------------------------------------------
// RE_Kenshi Plugin Entry Point
// ----------------------------------------------------------------------------

__declspec(dllexport) void startPlugin()
  {
    InitLogger("mods\\KenshiPython\\KenshiPy.log");

    Logger::Debug("KenshiPy startPlugin called.");
    
    if (!InstallHooks())
    {
        Logger::Error("Failure to install hooks.");
    }

    InitPython();
    Logger::Debug("KenshiPy initialization complete.");
    TryLoadMods();
}
