%{
#include "Debug.h"
%}

%include "std_string.i"

// Strip dllimport/dllexport so SWIG can parse the header
#define KLIB_EXPORT
#define __declspec(x)

%feature("docstring") DebugLog "Logs a debug message to RE_Kenshi_log.txt in the Kenshi's root directory. Logs will be prefixed with the module name, KenshiPy.";
%feature("docstring") ErrorLog "Logs an error message to RE_Kenshi_log.txt in the Kenshi's root directory. Logs will be prefixed with the module name, KenshiPy.";

%ignore GetModuleName;

%include "Debug.h"
