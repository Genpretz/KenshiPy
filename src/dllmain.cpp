#include "Hooks.h"
#include "PyRuntime.h"
#include "Logger.h"
#include "ScriptEditor.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <core/Functions.h>
#include "mygui/MyGUI_Gui.h"

#include "mygui/MyGUI_FontManager.h"
#include "kenshi/Kenshi.h"

// Forward declaration for debugging FontManager
namespace MyGUI
{
    class FontManager;
}

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
            Logger::Error("Failed to install hooks.");
            return;
        }
    }
    else
    {
        Logger::Error("Failed to initialize KenshiLib-py. Plugin cannot function.");
        return;
    }

    // Debugging MyGUI FontManager state
    try
    {
        Logger::Debug("Attempting to log MyGUI::FontManager state...");
        auto fontManager = MyGUI::FontManager::getInstancePtr();
        if (fontManager)
        {
            Logger::Debug("MyGUI::FontManager instance found.");
            // Add more detailed logging about FontManager state if possible
        }
        else
        {
            Logger::Error("MyGUI::FontManager instance is null.");
        }
    }
    catch (const std::exception& e)
    {
        Logger::Error("Exception while accessing MyGUI::FontManager: %s", e.what());
    }
    catch (...)
    {
        Logger::Error("Unknown exception while accessing MyGUI::FontManager.");
    }

    // Adding logs for options menu close button behavior
    Logger::Debug("Adding hooks or listeners for options menu close button...");
    // Example: Hook into the close button event if possible
    // This part depends on how MyGUI handles button events
    // If you have access to the options menu or close button, log its state here

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
