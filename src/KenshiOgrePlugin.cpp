#define OGRE_PLATFORM == OGRE_PLATFORM_WIN32

#include "KenshiOgrePlugin.h"
#include "KenshiPy_Runtime.h"

#include <core/Functions.h>
#include <kenshi/TitleScreen.h>


#include <Windows.h>

void (*initialise_orig)(wraps::BaseLayout* thisptr, const std::string& _layout, MyGUI::Widget* _parent, bool _throw, bool _createFakeWidgets) = nullptr;
void initialise_hook(wraps::BaseLayout* thisptr, const std::string& _layout, MyGUI::Widget* _parent, bool _throw, bool _createFakeWidgets)
{
    initialise_orig(thisptr, _layout, _parent, _throw, _createFakeWidgets);
    TryLoadMods();
}


TitleScreen* (*TitleScreen_orig)(TitleScreen*) = NULL;
TitleScreen* TitleScreen_hook(TitleScreen* thisptr)
{
    TitleScreen* result = TitleScreen_orig(thisptr);
    TryLoadMods();
    return result;
}

const String sPluginName = "KenshiPy";
//---------------------------------------------------------------------
KenshiOgrePlugin::KenshiOgrePlugin()
{
    OutputDebugStringA("KenshiOgrePlugin::KenshiOgrePlugin called\n");
}
//---------------------------------------------------------------------
const String& KenshiOgrePlugin::getName() const
{
    return sPluginName;
}
//---------------------------------------------------------------------
void KenshiOgrePlugin::install()
{
    KenshiLib::InitRVAs();
    OutputDebugStringA("KenshiOgrePlugin::install called\n");
    KenshiLib::AddHook(KenshiLib::GetRealAddress(&TitleScreen::_CONSTRUCTOR), TitleScreen_hook, &TitleScreen_orig);
    KenshiLib::AddHook(KenshiLib::GetRealAddress(&wraps::BaseLayout::initialise), initialise_hook, &initialise_orig);
}
//---------------------------------------------------------------------
void KenshiOgrePlugin::initialise()
{
    Init();
    OutputDebugStringA("KenshiOgrePlugin::initialise called\n");
}
//---------------------------------------------------------------------
void KenshiOgrePlugin::shutdown()
{
    OutputDebugStringA("KenshiOgrePlugin::shutdown called\n");
    //ShutdownPython();
}
//---------------------------------------------------------------------
void KenshiOgrePlugin::uninstall()
{
    OutputDebugStringA("KenshiOgrePlugin::uninstall called\n");
}
