#include "KenshiOgrePlugin.h"
#include "KenshiPy_Runtime.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <ogre/Ogre.h>

#include <boost/filesystem.hpp>

KenshiPy* KPyPlugin;

// ----------------------------------------------------------------------------
// Ogre Plugin System Entry Points
// ----------------------------------------------------------------------------

extern "C" __declspec(dllexport) void dllStartPlugin(void)
{
    KPyPlugin = new KenshiPy();
    Ogre::Root::getSingleton().installPlugin(KPyPlugin);
}

extern "C" __declspec(dllexport) void dllStopPlugin(void)
{
    Ogre::Root::getSingleton().uninstallPlugin(KPyPlugin);
    delete KPyPlugin;
    ShutdownPython();
}

// ----------------------------------------------------------------------------
// DLL Entry Point
// ----------------------------------------------------------------------------

namespace fs = boost::filesystem;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
    {
        char pluginPath[_MAX_PATH];
        GetModuleFileNameA(hModule, pluginPath, _MAX_PATH);
        fs::path s = pluginPath;
        SetDllDirectoryW( s.branch_path().c_str());
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
