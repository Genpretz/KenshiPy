#include "Hooks.h"
#include "Callbacks.h"
#include "ScriptEditor.h"
#include "Logger.h"

#include <core/Functions.h>
#include <kenshi/gui/TitleScreen.h>
#include <kenshi/InputHandler.h>
#include <kenshi/Character.h>
#include "kenshi\Inventory.h"

#include "mygui/MyGUI_Gui.h"

#include <cstddef>

// ---------------------------------------------------------------------------
// Generic hook installer
//
// Wraps KenshiLib::AddHook with the bookkeeping every hook needs:
//   - skip if already installed (orig pointer already set)
//   - bail with a logged error if the address can't be resolved
//   - bail with a logged error if AddHook itself fails
//   - log success
// ---------------------------------------------------------------------------

template <typename T>
static bool InstallHookT(const char* name, intptr_t addr, T hookFn, T* origStorage)
{
    if (*origStorage)
        return true;   // already installed

    if (!addr)
    {
        Logger::Error("Could not resolve address for %s.", name);
        return false;
    }

    KenshiLib::HookStatus status = KenshiLib::AddHook(addr, hookFn, origStorage);

    if (status != KenshiLib::SUCCESS)
    {
        Logger::Error("AddHook failed for %s (status %d).", name, (int)status);
        return false;
    }

    Logger::Debug("Hook installed: %s", name);
    return true;
}

// Defines a small static "installer" function for one hook, then provides
// an array-initializer fragment {name, fnPtr} referencing it.
#define DEFINE_HOOK_INSTALLER(fnName, displayName, addrExpr, hookFn, origVar) \
    static bool fnName() \
    { \
        return InstallHookT(displayName, (intptr_t)(addrExpr), &hookFn, &origVar); \
    }

// ---------------------------------------------------------------------------
// Hook: Character::_NV_select
//
// Fires when a character is selected. Run the original first, then dispatch
// to any Python-registered select callbacks.
// ---------------------------------------------------------------------------

static void (*Character_select_orig)(Character*) = nullptr;

static void _NV_select(Character* thisptr)
{
    Character_select_orig(thisptr);

    CallCharacterSelectCallbacks(thisptr);
}

DEFINE_HOOK_INSTALLER(InstallHook_Character_Select,
    "Character::_NV_select",
    KenshiLib::GetRealAddress(&Character::_NV_select),
    _NV_select, Character_select_orig)

    // ---------------------------------------------------------------------------
    // Hook: Character::_NV_unselect
    //
    // Fires when a character is unselected. Run the original first, then
    // dispatch to any Python-registered unselect callbacks.
    // ---------------------------------------------------------------------------

    static void (*Character_unselect_orig)(Character*) = nullptr;

static void _NV_unselect(Character* thisptr)
{
    Character_unselect_orig(thisptr);

    CallCharacterUnselectCallbacks(thisptr);
}

DEFINE_HOOK_INSTALLER(InstallHook_Character_Unselect,
    "Character::_NV_unselect",
    KenshiLib::GetRealAddress(&Character::_NV_unselect),
    _NV_unselect, Character_unselect_orig)

    // ---------------------------------------------------------------------------
    // Hook: Character::say
    //
    // Fires whenever a character speaks. Run the original first, then dispatch
    // to any Python-registered say callbacks with the message text.
    //
    // NOTE: Signature below is inferred from CallCharacterSayCallbacks(Character*,
    // const std::string&) in Callbacks.cpp. Verify against the real
    // Character::say signature in Character.h before building.
    // ---------------------------------------------------------------------------

    static void (*Character_say_orig)(Character*, const std::string&) = nullptr;

static void _NV_say(Character* thisptr, const std::string& msg)
{
    Character_say_orig(thisptr, msg);

    CallCharacterSayCallbacks(thisptr, msg);
}

DEFINE_HOOK_INSTALLER(InstallHook_Character_Say,
    "Character::say",
    KenshiLib::GetRealAddress(&Character::_NV_say),
    _NV_say, Character_say_orig)

    // ---------------------------------------------------------------------------
    // Hook: Character::declareDead
    //
    // Fires when a character dies. Run the original first, then dispatch to any
    // Python-registered declareDead callbacks.
    //
    // NOTE: Signature below is inferred from CallCharacterDeclareDeadCallbacks(
    // Character*) in Callbacks.cpp. Verify against the real
    // Character::declareDead signature (and exact symbol name — it may not be
    // "declareDead" in the binary) in Character.h before building.
    // ---------------------------------------------------------------------------

    static void (*Character_declareDead_orig)(Character*) = nullptr;

static void _NV_declareDead(Character* thisptr)
{
    Character_declareDead_orig(thisptr);

    CallCharacterDeclareDeadCallbacks(thisptr);
}

DEFINE_HOOK_INSTALLER(InstallHook_Character_DeclareDead,
    "Character::declareDead",
    KenshiLib::GetRealAddress(&Character::declareDead),
    _NV_declareDead, Character_declareDead_orig)

    // ---------------------------------------------------------------------------
    // Hook: InputHandler::keyDownEvent
    //
    // Called by the game for every key press. We intercept it to:
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

DEFINE_HOOK_INSTALLER(InstallHook_InputHandler_KeyDown,
    "InputHandler::keyDownEvent",
    KenshiLib::GetRealAddress(&InputHandler::keyDownEvent),
    InputHandler_keyDownEvent_hook, InputHandler_keyDownEvent_orig)

    // ---------------------------------------------------------------------------
    // Hook: TitleScreen constructor
    //
    // The TitleScreen is the first thing that creates the MyGUI environment in a
    // usable state. We piggyback on its constructor to schedule our own GUI
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

DEFINE_HOOK_INSTALLER(InstallHook_TitleScreen,
    "TitleScreen::_CONSTRUCTOR",
    KenshiLib::GetRealAddress(&TitleScreen::_CONSTRUCTOR),
    TitleScreen_hook, TitleScreen_orig)

    // ---------------------------------------------------------------------------
    // Hook:: Inventory::takeItem
    //
    // ---------------------------------------------------------------------------

static void (*Inventory_takeMoney_orig)(Inventory* thisptr, int val) = nullptr;

static void Inventory_takeMoney_hook(Inventory* thisptr, int val)
{
    Inventory_takeMoney_orig(thisptr, val);

    CallTakeMoneyCallbacks(thisptr, val);
}

DEFINE_HOOK_INSTALLER(InstallHook_Inventory_TakeMoney,
    "Inventory::takeMoney",
    KenshiLib::GetRealAddress(&Inventory::takeMoney),
    Inventory_takeMoney_hook, Inventory_takeMoney_orig)

    // ---------------------------------------------------------------------------
    // Hook registry
    //
    // Each entry resolves an address, installs the trampoline, and logs the
    // result. Adding a new hook is a one-line addition here plus the trampoline
    // function above it — no separate HookXxx() function required.
    // ---------------------------------------------------------------------------

    struct HookRegistryEntry
{
    const char* name;
    bool (*install)();
};

static const HookRegistryEntry g_hookRegistry[] = {

    { "TitleScreen::_CONSTRUCTOR",   InstallHook_TitleScreen },
    { "InputHandler::keyDownEvent",  InstallHook_InputHandler_KeyDown },
    { "Character::_NV_select",       InstallHook_Character_Select },
    { "Character::_NV_unselect",     InstallHook_Character_Unselect },
    { "Character::say",              InstallHook_Character_Say },
    { "Character::declareDead",      InstallHook_Character_DeclareDead },
    { "Inventory::takeMoney",        InstallHook_Inventory_TakeMoney }
};

static const size_t g_hookRegistryCount = sizeof(g_hookRegistry) / sizeof(g_hookRegistry[0]);

// ---------------------------------------------------------------------------
// Public entry point
// ---------------------------------------------------------------------------

bool InstallHooks()
{
    Logger::Debug("Installing hooks...");

    bool ok = true;
    for (size_t i = 0; i < g_hookRegistryCount; ++i)
    {
        if (!g_hookRegistry[i].install())
            ok = false;
    }

    if (ok)
        Logger::Debug("All hooks installed successfully.");
    else
        Logger::Error("One or more hooks failed, see errors above.");

    return ok;
}
