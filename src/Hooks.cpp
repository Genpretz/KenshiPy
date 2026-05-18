#include "Hooks.h"
#include "Callbacks.h"
#include "ScriptEditor.h"
#include "Logger.h"

#include <core/Functions.h>
#include <kenshi/gui/TitleScreen.h>
#include <kenshi/InputHandler.h>

#include "mygui/MyGUI_Gui.h"

// ---------------------------------------------------------------------------
// Hook: InputHandler::keyDownEvent
//
// Called by the game for every key press.  We intercept it to:
//   1. Run the original handler first (always).
//   2. Toggle the script editor on Ctrl+`.
//   3. Dispatch to any Python key-down callbacks.
// ---------------------------------------------------------------------------

static void (*InputHandler_keyDownEvent_orig)(InputHandler*, OIS::KeyCode) = nullptr;

static void InputHandler_keyDownEvent_hook(InputHandler* self, OIS::KeyCode key)
{
    // Always run the original handler so the game still receives input.
    InputHandler_keyDownEvent_orig(self, key);

    // Ctrl+` — toggle the script editor.
    if (key == OIS::KC_GRAVE && self->ctrl)
        ScriptEditor::Toggle();

    // Dispatch to any Python-registered key-down callbacks.
    CallKeyDownCallbacks(static_cast<int>(key));
}

// ---------------------------------------------------------------------------
// Hook: TitleScreen constructor
//
// The TitleScreen is the first thing that creates the MyGUI environment in a
// usable state.  We piggyback on its constructor to schedule our own GUI
// initialisation via MyGUI's per-frame event (which fires after the first
// full layout pass).
// ---------------------------------------------------------------------------

static TitleScreen* (*TitleScreen_orig)(TitleScreen*) = nullptr;

static TitleScreen* TitleScreen_hook(TitleScreen* self)
{
    TitleScreen* result = TitleScreen_orig(self);

    MyGUI::Gui* gui = MyGUI::Gui::getInstancePtr();
    if (gui && !ScriptEditor::IsInitialized())
        gui->eventFrameStart += MyGUI::newDelegate(ScriptEditor::InitFrameHandler);

    return result;
}

// ---------------------------------------------------------------------------
// Hook installation helpers
// ---------------------------------------------------------------------------

static bool HookInputHandler()
{
    if (InputHandler_keyDownEvent_orig)
        return true;   // already installed

    intptr_t addr = KenshiLib::GetRealAddress(&InputHandler::keyDownEvent);
    if (!addr)
    {
        Logger::Error("Could not resolve InputHandler::keyDownEvent address.");
        return false;
    }

    KenshiLib::HookStatus status = KenshiLib::AddHook(
        addr,
        &InputHandler_keyDownEvent_hook,
&InputHandler_keyDownEvent_orig);

    if (status != KenshiLib::SUCCESS)
    {
        Logger::Error("AddHook failed for InputHandler::keyDownEvent (status %d).", (int)status);
        return false;
    }

    Logger::Debug("Hook installed: InputHandler::keyDownEvent");
    return true;
}

static bool HookTitleScreen()
{
    if (TitleScreen_orig)
        return true;

    intptr_t addr = KenshiLib::GetRealAddress(&TitleScreen::_CONSTRUCTOR);
    if (!addr)
    {
        Logger::Error("Could not resolve TitleScreen::_CONSTRUCTOR address.");
        return false;
    }

    KenshiLib::HookStatus status = KenshiLib::AddHook(
        addr, &TitleScreen_hook,
    &TitleScreen_orig);

    if (status != KenshiLib::SUCCESS)
    {
        Logger::Error("AddHook failed for TitleScreen::_CONSTRUCTOR (status %d).", (int)status);
        return false;
    }

    Logger::Debug("Hook installed: TitleScreen::_CONSTRUCTOR");
    return true;
}

// ---------------------------------------------------------------------------
// Public entry point
// ---------------------------------------------------------------------------

bool InstallHooks()
{
    Logger::Debug("Installing hooks...");
    bool ok = HookTitleScreen() && HookInputHandler();
    if (ok)
        Logger::Debug("All hooks installed successfully.");
    else
        Logger::Error("One or more hooks failed — see errors above.");
    return ok;
}
