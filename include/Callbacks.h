#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <kenshi\Character.h>

// ---------------------------------------------------------------------------
// Python-callable event callbacks
//
// Scripts register callables here; the hooks call the C++ dispatcher which
// acquires the GIL and invokes each callable.
// ---------------------------------------------------------------------------

// Key-down — callable receives a single int (OIS key code).
void RegisterKeyDownCallback  (PyObject* callable);
void UnregisterKeyDownCallback(PyObject* callable);
void CallKeyDownCallbacks     (int keyCode);

// Per-frame — callable receives a single float (delta time in seconds).
void RegisterFrameCallback    (PyObject* callable);
void UnregisterFrameCallback  (PyObject* callable);
// (CallFrameCallbacks is invoked by the internal Ogre FrameListener.)

void RegisterCharacterSayCallback(PyObject* callable);
void UnregisterCharacterSayCallback(PyObject* callable);
void CallCharacterSayCallbacks(Character* c, const std::string& msg);

void RegisterCharacterSelectCallback(PyObject* callable);
void UnregisterCharacterSelectCallback(PyObject* callable);
void CallCharacterSelectCallbacks(Character* c);

void RegisterCharacterUnselectCallback(PyObject* callable);
void UnregisterCharacterUnselectCallback(PyObject* callable);
void CallCharacterUnselectCallbacks(Character* c);

void RegisterCharacterDeclareDeadCallback(PyObject* callable);
void UnregisterCharacterDeclareDeadCallback(PyObject* callable);
void CallCharacterDeclareDeadCallbacks(Character* c);



