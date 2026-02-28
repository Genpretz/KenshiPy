%{
#include "SensoryData.h"
%}

%include "std_string.i"

%ignore SeenSomeone::lastSeenTime;
%ignore SeenSomeone::lastPosition;

// SenseItr constructor uses ogre_unordered_map - not wrappable
%ignore SenseItr::SenseItr;
%ignore SenseItr::_CONSTRUCTOR;

// TagsClass template methods not exported from KenshiLib.lib
%ignore TagsClass;

%include "SensoryData.h"
