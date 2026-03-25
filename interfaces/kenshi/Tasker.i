%{
#include "kenshi/Tasker.h"
%}

%include "std_string.i"

%ignore TaskData::setTargetingFunction;
%ignore TaskData::_CONSTRUCTOR;
%ignore TaskData::TaskData;

%include "kenshi/Tasker.h"
