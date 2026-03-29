#pragma once
#define PY_SSIZE_T_CLEAN
#include <Python.h>

void RegisterKeyDownCallback(PyObject* callable);
void UnregisterKeyDownCallback(PyObject* callable);
void CallKeyDownCallbacks(int keyCode);

void RegisterFrameCallback(PyObject* callable);
void UnregisterFrameCallback(PyObject* callable);
void CallFrameCallbacks(float deltaTime);

