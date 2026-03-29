#include "Hooks.h"

#include "Callbacks.h"
#include "Console.h"
#include "KenshiPy_Runtime.h"
#include "Logger.h"

#include <core/Functions.h>
#include <kenshi/gui/TitleScreen.h>
#include <kenshi/InputHandler.h>
#include <kenshi/Globals.h>

#include "mygui/MyGUI_Gui.h"
#include "mygui/MyGUI_Widget.h"

void (*InputHandler_keyDownEvent_orig)(InputHandler*, OIS::KeyCode) = NULL;
void InputHandler_keyDownEvent_hook(InputHandler* thisptr, OIS::KeyCode keyCode)
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

    if (!Console::IsInitialized())
    {
        MyGUI::Gui* gui = MyGUI::Gui::getInstancePtr();
        if (gui)
            gui->eventFrameStart += MyGUI::newDelegate(Console::InitFrameHandler);
    }

    return result;
}

bool installInputHandlerHooks()
{
    if (KenshiLib::SUCCESS != KenshiLib::AddHook(KenshiLib::GetRealAddress(&InputHandler::keyDownEvent), InputHandler_keyDownEvent_hook, &InputHandler_keyDownEvent_orig))
    {
        ErrorLog1("Failed to hook InputHandler::keyDownEvent.");
        return false;
    }
    return true;
}

bool installTitlescreenHooks()
{
    if (KenshiLib::SUCCESS != KenshiLib::AddHook(KenshiLib::GetRealAddress(&TitleScreen::_CONSTRUCTOR), TitleScreen_hook, &TitleScreen_orig))
    {
        ErrorLog1("Failed to hook Titlescreen::_CONSTRUCTOR.");
        return false;
    }

    return true;
}