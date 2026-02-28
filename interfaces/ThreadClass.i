%{
#include "ThreadClass.h"
%}

%include "std_string.i"

// Suppress Boost shared_mutex members -- not wrappable via SWIG
%ignore ThreadClass::runMute;
%ignore ThreadClass::lockedWhileRunningMute;
%nocopyctor ThreadClass;

%include "ThreadClass.h"
