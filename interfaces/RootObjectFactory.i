%{
#include "RootObjectFactory.h"
%}

%include "std_string.i"

%ignore RootObjectFactory::todoMutex;
%nocopyctor RootObjectFactory;

%include "RootObjectFactory.h"
