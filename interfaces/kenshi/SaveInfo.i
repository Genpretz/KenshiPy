%{
#include "kenshi/SaveInfo.h"
%}

%include "std_string.i"

// SaveInfo constructors not exported from KenshiLib.lib
%ignore SaveInfo::SaveInfo;
%ignore SaveInfo::_CONSTRUCTOR;

%include "kenshi/SaveInfo.h"
