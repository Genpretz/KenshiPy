%{
#include "kenshi/RootObjectFactory.h"
%}

%include "std_string.i"

%ignore RootObjectFactory::todoMutex;
%nocopyctor RootObjectFactory;

%include "kenshi/RootObjectFactory.h"
