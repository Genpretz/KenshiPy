#include "Hooks.h"
#include "Callbacks.h"
#include "ScriptEditor.h"
#include "Logger.h"

#include <core/Functions.h>
#include <kenshi/gui/TitleScreen.h>
#include <kenshi/InputHandler.h>

#include "mygui/MyGUI_Gui.h"

static void (*Character_declareDead_orig)(Character* thisptr) = nullptr;

static void Character_declareDead_hook(Character* thisptr)
{
    Character_declareDead_orig(thisptr);

    CallCharacterDeclareDeadCallbacks(thisptr);
}

static void (*Character_NV_say_orig)(Character*, const std::string& msg) = nullptr;

static void Character_NV_say_hook(Character* thisptr, const std::string& msg)
{
    Character_NV_say_orig(thisptr, msg);

    CallCharacterSayCallbacks(thisptr, msg);
}

static void (*Character_NV_select_orig)(Character*) = nullptr;

static void Character_NV_select_hook(Character* thisptr)
{
    Character_NV_select_orig(thisptr);

    CallCharacterSelectCallbacks(thisptr);
}

static void (*Character_NV_unselect_orig)(Character*) = nullptr;

static void Character_NV_unselect_hook(Character* thisptr)
{
    Character_NV_unselect_orig(thisptr);

    CallCharacterUnselectCallbacks(thisptr);
}

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
// initialisation via MyGUI's per-frame event.
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

static bool HookCharacterUnselect()
{
    if (Character_NV_unselect_orig)
        return true;

    intptr_t addr = KenshiLib::GetRealAddress(&Character::_NV_unselect);
    if (!addr)
    {
        Logger::Error("Could not resolve Character::_NV_unselect address.");
        return false;
    }

    KenshiLib::HookStatus status = KenshiLib::AddHook(
        addr, &Character_NV_unselect_hook,
        &Character_NV_unselect_orig);

    if (status != KenshiLib::SUCCESS)
    {
        Logger::Error("AddHook failed for Character::_NV_unselect (status %d).", (int)status);
        return false;
    }

    Logger::Debug("Hook installed: Character::_NV_unselect");
    return true;
}

static bool HookCharacterSelect()
{
    if (Character_NV_select_orig)
        return true;

    intptr_t addr = KenshiLib::GetRealAddress(&Character::_NV_select);
    if (!addr)
    {
        Logger::Error("Could not resolve Character::_NV_select address.");
        return false;
    }

    KenshiLib::HookStatus status = KenshiLib::AddHook(
        addr, &Character_NV_select_hook,
        &Character_NV_select_orig);

    if (status != KenshiLib::SUCCESS)
    {
        Logger::Error("AddHook failed for Character::_NV_select (status %d).", (int)status);
        return false;
    }

    Logger::Debug("Hook installed: Character::_NV_select");
    return true;
}

static bool HookCharacterSay()
{
    if (Character_NV_say_orig)
        return true;

    intptr_t addr = KenshiLib::GetRealAddress(&Character::_NV_say);
    if (!addr)
    {
        Logger::Error("Could not resolve Character::_NV_say address.");
        return false;
    }

    KenshiLib::HookStatus status = KenshiLib::AddHook(
        addr, &Character_NV_say_hook,
        &Character_NV_say_orig);

    if (status != KenshiLib::SUCCESS)
    {
        Logger::Error("AddHook failed for Character::_NV_say (status %d).", (int)status);
        return false;
    }

    Logger::Debug("Hook installed: Character::_NV_say");
    return true;
}

static bool HookDeclareDead()
{
    if (Character_declareDead_orig)
        return true;

    intptr_t addr = KenshiLib::GetRealAddress(&Character::declareDead);
    if (!addr)
    {
        Logger::Error("Could not resolve Character::declareDead address.");
        return false;
    }

    KenshiLib::HookStatus status = KenshiLib::AddHook(
        addr, &Character_declareDead_hook,
        &Character_declareDead_orig);

    if (status != KenshiLib::SUCCESS)
    {
        Logger::Error("AddHook failed for Character::declareDead (status %d).", (int)status);
        return false;
    }

    Logger::Debug("Hook installed: Character::declareDead");
    return true;
}

// ---------------------------------------------------------------------------
// Public entry point
// ---------------------------------------------------------------------------

bool InstallHooks()
{
    Logger::Debug("Installing hooks...");
    bool ok = HookTitleScreen() && HookInputHandler() && HookDeclareDead() && HookCharacterSelect() && HookCharacterUnselect() && HookCharacterSay();
    if (ok)
        Logger::Debug("All hooks installed successfully.");
    else
        Logger::Error("One or more hooks failed — see errors above.");
    return ok;
}