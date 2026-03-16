%{
#include "kenshi/RootObjectBase.h"
%}

%include "std_string.i"

%ignore RootObjectBase::pos;
%ignore RootObjectBase::handle;

%include "kenshi/RootObjectBase.h"
