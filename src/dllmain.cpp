#include "Hooks.h"
#include "PyRuntime.h"
#include "Logger.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <boost/filesystem.hpp>
#include <core/Functions.h>
#include "mygui/MyGUI_Gui.h"

// ----------------------------------------------------------------------------
// Hook installation with delayed retry
// ----------------------------------------------------------------------------
static bool g_hooksInstalled = false;
static int g_frameCount = 0;
static const int HOOK_RETRY_FRAME = 5;  // Try again after 5 frames

void HookRetryFrameHandler(float /*timeDelta*/)
{
    g_frameCount++;

    if (g_frameCount < HOOK_RETRY_FRAME)
        return;

    Logger::DebugLog("Retrying hook installation (frame %d)...", g_frameCount);

    bool titlescreenOk = installTitlescreenHooks();
    bool inputOk = installInputHandlerHooks();

    if (titlescreenOk && inputOk)
    {
        g_hooksInstalled = true;
        Logger::DebugLog("All hooks installed successfully on retry.");

        MyGUI::Gui* gui = MyGUI::Gui::getInstancePtr();
        if (gui)
            gui->eventFrameStart -= MyGUI::newDelegate(HookRetryFrameHandler);
    }
    else if (g_frameCount > 60)  // Stop trying after ~60 frames
    {
        Logger::ErrorLog("Hook installation failed after multiple retries. Giving up.");

        MyGUI::Gui* gui = MyGUI::Gui::getInstancePtr();
        if (gui)
            gui->eventFrameStart -= MyGUI::newDelegate(HookRetryFrameHandler);
    }
}

// ----------------------------------------------------------------------------
// RE_Kenshi Plugin Entry Point
// ----------------------------------------------------------------------------

__declspec(dllexport) void startPlugin()
{
    Logger::InitLogger("mods\\KenshiPython\\KenshiPy.log");
    Logger::DebugLog("KenshiPy startPlugin called.");
    HMODULE kenshiLib = GetModuleHandleA("KenshiLib.dll");
    Logger::DebugLog("KenshiLib.dll handle: 0x%p", kenshiLib);
    MyGUI::Gui* gui = nullptr;

    while (gui == nullptr)
    {
        Logger::DebugLog("Waiting for MyGUI::Gui instance...");
        gui = MyGUI::Gui::getInstancePtr();
    }

    Logger::DebugLog("MyGUI::Gui instance found.");
    Logger::DebugLog("Attempting to install hooks...");

    bool titlescreenOk = installTitlescreenHooks();
    bool inputOk = installInputHandlerHooks();

    if (titlescreenOk && inputOk)
    {
        g_hooksInstalled = true;
        Logger::DebugLog("Hooks installed successfully.");
    }
    else
    {
        Logger::DebugLog("Initial hook installation failed. Will retry after a few frames...");
        gui->eventFrameStart += MyGUI::newDelegate(HookRetryFrameHandler);
    }

    // Initialize Python runtime
    InitPython();
    Logger::DebugLog("KenshiPy initialization complete.");
    TryLoadMods();
}

// ----------------------------------------------------------------------------
// DLL Entry Point
// ----------------------------------------------------------------------------

namespace fs = boost::filesystem;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  fdwReason, LPVOID lpvReserved)
{
    return TRUE;
}