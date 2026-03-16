%{
#include "core\Functions.h"
%}

#define KLIB_EXPORT
#define __declspec(x)

%include "core\Functions.h"

%inline %{
    // AddHookByAddress accepts raw integer addresses so Python can pass values
    // from GetRealAddress or ctypes without needing SWIG-wrapped pointer objects.
    // Returns the trampoline (original function) address as a plain integer,
    // or 0 on failure. Does not modify the original AddHook signature.
    intptr_t AddHookByAddress(intptr_t target, intptr_t detour)
    {
        void* orig = NULL;
        KenshiLib::HookStatus status = KenshiLib::AddHook((void*)target, (void*)detour, &orig);
        if (status == KenshiLib::FAIL)
            return 0;
        return (intptr_t)orig;
    }
%}
