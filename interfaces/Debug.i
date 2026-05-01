%{
#include "Debug.h"
%}

%include "std_string.i"

// Strip dllimport/dllexport so SWIG can parse the header
#define KLIB_EXPORT
#define __declspec(x)
//%ignore GetModuleName;
%feature("docstring") DebugLog "Logs a debug message to KenshiPy_log.txt in the KenshiPython directory.";
%feature("docstring") ErrorLog "Logs an error message to KenshiPy_log.txt in the KenshiPython directory.";

%include "Debug.h"
