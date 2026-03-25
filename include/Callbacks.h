#pragma once

void RegisterKeyDownCallback(PyObject* callable);
void UnregisterKeyDownCallback(PyObject* callable);
void CallKeyDownCallbacks(int keyCode);

void RegisterFrameCallback(PyObject* callable);
void UnregisterFrameCallback(PyObject* callable);
void CallFrameCallbacks(float deltaTime);

