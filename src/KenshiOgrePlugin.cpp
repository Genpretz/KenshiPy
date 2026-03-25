#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define OGRE_PLATFORM == OGRE_PLATFORM_WIN32

#include "Console.h"
#include "KenshiOgrePlugin.h"
#include "KenshiPy_Runtime.h"
#include "Callbacks.h"

#include <Debug.h>
#include <core/Functions.h>
#include <kenshi/gui/TitleScreen.h>
#include <kenshi/InputHandler.h>
#include <kenshi/Globals.h>
#include <ois/OISKeyboard.h>

#include "mygui/MyGUI_Gui.h"
#include "mygui/MyGUI_Widget.h"

static bool g_consoleInitialized = false;

static MyGUI::Widget* FindWidget(MyGUI::EnumeratorWidgetPtr enumerator, const std::string& name)
{
    while (enumerator.next())
    {
        MyGUI::Widget* w = enumerator.current();
        const std::string& widgetName = w->getName();
        if (widgetName == name)
            return w;
        size_t splitPos = widgetName.find('_');
        if (splitPos != std::string::npos && widgetName.substr(splitPos + 1) == name)
            return w;
        if (w->getChildCount() > 0)
        {
            MyGUI::Widget* found = FindWidget(w->getEnumerator(), name);
            if (found != NULL)
                return found;
        }
    }
    return NULL;
}

static void ConsoleInitFrameHandler(float timeDelta)
{
    MyGUI::Gui* gui = MyGUI::Gui::getInstancePtr();
    if (!gui)
        return;

    MyGUI::Widget* versionText = FindWidget(gui->getEnumerator(), "VersionText");
    if (!versionText)
        return;

    // VersionText exists — GUI is ready
    gui->eventFrameStart -= MyGUI::newDelegate(ConsoleInitFrameHandler);
    g_consoleInitialized = true;
    Console::Init();
}

static void (*InputHandler_keyDownEvent_orig)(InputHandler*, OIS::KeyCode) = NULL;
static void InputHandler_keyDownEvent_hook(InputHandler* thisptr, OIS::KeyCode keyCode)
{
    if (InputHandler_keyDownEvent_orig)
        InputHandler_keyDownEvent_orig(thisptr, keyCode);
    if (keyCode == OIS::KC_GRAVE && thisptr->alt)
        Console::Toggle();
    CallKeyDownCallbacks((int)keyCode);
}


TitleScreen* (*TitleScreen_orig)(TitleScreen*) = NULL;
TitleScreen* TitleScreen_hook(TitleScreen* thisptr)
{
    TitleScreen* result = TitleScreen_orig(thisptr);
    TryLoadMods();

    if (!g_consoleInitialized)
    {
        MyGUI::Gui* gui = MyGUI::Gui::getInstancePtr();
        if (gui)
            gui->eventFrameStart += MyGUI::newDelegate(ConsoleInitFrameHandler);
    }

    return result;
}

const String sPluginName = "KenshiPy";

KenshiOgrePlugin::KenshiOgrePlugin()
{
    OutputDebugStringA("KenshiOgrePlugin::KenshiOgrePlugin called\n");
}

const String& KenshiOgrePlugin::getName() const
{
    return sPluginName;
}

void KenshiOgrePlugin::install()
{
    KenshiLib::InitRVAs();
    OutputDebugStringA("KenshiOgrePlugin::install called\n");
    KenshiLib::AddHook(KenshiLib::GetRealAddress(&TitleScreen::_CONSTRUCTOR), TitleScreen_hook, &TitleScreen_orig);
    KenshiLib::AddHook(KenshiLib::GetRealAddress(&InputHandler::keyDownEvent), InputHandler_keyDownEvent_hook, &InputHandler_keyDownEvent_orig);
}

void KenshiOgrePlugin::initialise()
{
    InitPython();
    OutputDebugStringA("KenshiOgrePlugin::initialise called\n");
}

void KenshiOgrePlugin::shutdown()
{
    OutputDebugStringA("KenshiOgrePlugin::shutdown called\n");
    ShutdownPython();
}

void KenshiOgrePlugin::uninstall()
{
    OutputDebugStringA("KenshiOgrePlugin::uninstall called\n");
}
