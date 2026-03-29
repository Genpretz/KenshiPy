#include "KenshiOgrePlugin.h"

#include "Callbacks.h"
#include "Console.h"
#include "Hooks.h"
#include "KenshiPy_Runtime.h"
#include "Logger.h"
//#include <Debug.h>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <ogre/Ogre.h>
#include <ois/OISKeyboard.h>

#include <core/Functions.h>

const String s_PluginName = "KenshiPy";
const String s_PluginVersion = "0.1.2-alpha";
bool g_initializedKenshiLib = false;

KenshiPy::KenshiPy()
{
    String sPlugin = (s_PluginName + " " + s_PluginVersion);
    DebugLog1(sPlugin);
}

const String& KenshiPy::getName() const
{
    return s_PluginName;
}

const String& KenshiPy::getVersion() const
{
    return s_PluginVersion;
}

void KenshiPy::install()
{
    DebugLog1("KenshiPy::install called.");
    DebugLog1("Initializing KenshiLib...");

    if (!KenshiLib::InitRVAs())
    {
        DebugLog1("Failed to initialize KenshiLib.");
        return;
    }

    g_initializedKenshiLib = true;
    DebugLog1("KenshiLib initialized successfully.");
    
    DebugLog1("Attempting to install hooks...");

    bool installedHooks = true;

    if (!installTitlescreenHooks())
    {
        installedHooks = false;
    }
    if (!installInputHandlerHooks())
    {
        installedHooks = false;
    }
    if (installedHooks)
    {
        DebugLog1("Hooks installed successfully.");
    }
}

void KenshiPy::initialise()
{
    if (!g_initializedKenshiLib)
    {
        DebugLog1("KenshiPy::initialise called but KenshiLib was not initialized successfully. Skipping Python initialization.");
        return;
	}
    DebugLog1("KenshiPy::initialise called. Beginning Python initialization...");
    InitPython();
}

void KenshiPy::shutdown()
{
    DebugLog1("KenshiPy::shutdown called.");
}

void KenshiPy::uninstall()
{
    DebugLog1("KenshiPy::uninstall called.");
}
