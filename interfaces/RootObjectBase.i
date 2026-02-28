%{
#include "RootObjectBase.h"
%}

%include "std_string.i"

%ignore RootObjectBase::pos;
%ignore RootObjectBase::handle;

%include "RootObjectBase.h"
