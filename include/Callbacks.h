#pragma once

#include <Python.h>
#include <string>

class Character;
class Inventory;

#define DECLARE_CALLBACK_REGISTRATION(Name) \
    void Register##Name##Callback(PyObject* callable); \
    void Unregister##Name##Callback(PyObject* callable);

DECLARE_CALLBACK_REGISTRATION(KeyDown)
void CallKeyDownCallbacks(int keyCode);

DECLARE_CALLBACK_REGISTRATION(CharacterSay)
void CallCharacterSayCallbacks(Character* c, const std::string& msg);

DECLARE_CALLBACK_REGISTRATION(CharacterSelect)
void CallCharacterSelectCallbacks(Character* c);

DECLARE_CALLBACK_REGISTRATION(CharacterUnselect)
void CallCharacterUnselectCallbacks(Character* c);

DECLARE_CALLBACK_REGISTRATION(CharacterDeclareDead)
void CallCharacterDeclareDeadCallbacks(Character* c);

DECLARE_CALLBACK_REGISTRATION(TakeMoney)
void CallTakeMoneyCallbacks(Inventory* inv, int val);

DECLARE_CALLBACK_REGISTRATION(Frame)
// CallFrameCallbacks is internal-only — not declared here.