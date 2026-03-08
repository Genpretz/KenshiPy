%{
#include "core\Functions.h"
%}

#define KLIB_EXPORT
#define __declspec(x)

%include "core\Functions.h"

%inline %{
    // Accept raw integer addresses so Python can pass values from
    // GetRealAddress or ctypes without needing SWIG-wrapped pointer objects.
    KenshiLib::HookStatus AddHookByAddress(intptr_t target, intptr_t detour, intptr_t* original)
    {
        void* orig = NULL;
        KenshiLib::HookStatus status = KenshiLib::AddHook((void*)target, (void*)detour, &orig);
        *original = (intptr_t)orig;
        return status;
    }
%}