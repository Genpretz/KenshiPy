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

#include "core/Functions.h"

const String s_PluginName = "KenshiPy";
const String s_PluginVersion = "0.1.2-alpha";
bool g_initializedKenshiLib = false;

KenshiPy::KenshiPy()
{
    String sPlugin = (s_PluginName + " " + s_PluginVersion);
    Logger::DebugLog(sPlugin);
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
    Logger::DebugLog("KenshiPy::install called.");
    Logger::DebugLog("Initializing KenshiLib...");

    if (!KenshiLib::InitRVAs())
    {
        Logger::ErrorLog("Failed to initialize KenshiLib.");
        return;
    }

    g_initializedKenshiLib = true;
    Logger::DebugLog("KenshiLib initialized successfully.");
    
    Logger::DebugLog("Attempting to install hooks...");

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
        Logger::DebugLog("Hooks installed successfully.");
    }
}

void KenshiPy::initialise()
{
    Logger::DebugLog("KenshiPy::initialise called.");
    if (!g_initializedKenshiLib)
    {
        Logger::ErrorLog("KenshiLib was not initialized successfully. Skipping Python initialization.");
        return;
	}
    InitPython();
}

void KenshiPy::shutdown()
{
    Logger::DebugLog("KenshiPy::shutdown called.");
}

void KenshiPy::uninstall()
{
    Logger::DebugLog("KenshiPy::uninstall called.");
}
