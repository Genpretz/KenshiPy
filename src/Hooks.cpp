#include "Hooks.h"

#include "Callbacks.h"
#include "Console.h"
#include "Logger.h"

#include <core/Functions.h>
#include <kenshi/gui/TitleScreen.h>
#include <kenshi/InputHandler.h>

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
    //if (InputHandler_keyDownEvent_orig != NULL)
    //{
    //    Logger::DebugLog("InputHandler hook already installed.");
    //    return true;
    //}

    intptr_t targetAddr = KenshiLib::GetRealAddress(&InputHandler::keyDownEvent);
    Logger::DebugLog("InputHandler::keyDownEvent target address: 0x%p", (void*)targetAddr);

    //if (targetAddr == 0)
    //{
    //    Logger::ErrorLog("Failed to resolve InputHandler::keyDownEvent address (got 0)");
    //    return false;
    //}

    KenshiLib::HookStatus status = KenshiLib::AddHook(KenshiLib::GetRealAddress(&InputHandler::keyDownEvent),
        &InputHandler_keyDownEvent_hook, &InputHandler_keyDownEvent_orig);

    if (status != KenshiLib::SUCCESS)
    {
        Logger::ErrorLog("Failed to hook InputHandler::keyDownEvent. MinHook error: %d", (int)status);
        return false;
    }

    Logger::DebugLog("InputHandler::keyDownEvent hooked successfully.");
    return true;
}

bool installTitlescreenHooks()
{
    //if (TitleScreen_orig != NULL)
    //{
    //    Logger::DebugLog("TitleScreen hook already installed.");
    //    return true;
    //}

    intptr_t targetAddr = KenshiLib::GetRealAddress(&TitleScreen::_CONSTRUCTOR);
    Logger::DebugLog("TitleScreen::_CONSTRUCTOR target address: 0x%p", (void*)targetAddr);

    //if (targetAddr == 0)
    //{
    //    Logger::ErrorLog("Failed to resolve TitleScreen::_CONSTRUCTOR address (got 0)");
    //    return false;
    //}

    KenshiLib::HookStatus status = KenshiLib::AddHook(KenshiLib::GetRealAddress(&TitleScreen::_CONSTRUCTOR),
        &TitleScreen_hook,&TitleScreen_orig);

    if (status != KenshiLib::SUCCESS)
    {
        Logger::ErrorLog("Failed to hook Titlescreen::_CONSTRUCTOR. MinHook error: %d", (int)status);
        return false;
    }

    Logger::DebugLog("TitleScreen::_CONSTRUCTOR hooked successfully.");
    return true;
}