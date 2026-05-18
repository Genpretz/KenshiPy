// Functions.i - Simple wrapper for KenshiLib hooking functions

%{
#include "core/Functions.h"
%}

%include "stdint.i"

// Handle void pointers
%typemap(in) void* {
    $1 = PyLong_AsVoidPtr($input);
}

%typemap(out) void* {
    $result = PyLong_FromVoidPtr($1);
}

%typemap(out) intptr_t {
    $result = PyLong_FromVoidPtr((void*)$1);
}

// Handle the output parameter for original function pointer
%typemap(in, numinputs=0) void** original (void* temp) {
    $1 = &temp;
}

%typemap(argout) void** original {
    PyObject* ptr = PyLong_FromVoidPtr(*$1);
    $result = SWIG_Python_AppendOutput($result, ptr);
}

namespace KenshiLib {
    enum HookStatus {
        SUCCESS = 0,
        FAIL = 1
    };
    
    intptr_t GetRealAddress(void* fun);
    HookStatus AddHook(void* target, void* detour, void** original);
}