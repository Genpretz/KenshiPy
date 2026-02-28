%{
#include "Debug.h"
%}

%include "std_string.i"

// Strip dllimport/dllexport so SWIG can parse the header
#define KLIB_EXPORT
#define __declspec(x)

%include "Debug.h"
