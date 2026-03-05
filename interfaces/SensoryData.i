%{
#include "SensoryData.h"
%}

%include "std_string.i"

%import "TagsClass.h"

%ignore SeenSomeone::lastSeenTime;
%ignore SeenSomeone::lastPosition;

// TagsClass<SenseType> member - not wrappable (template, no copy constructor/operator=)
%ignore SeenSomeone::type;

// SenseItr constructor uses ogre_unordered_map - not wrappable
%ignore SenseItr::SenseItr;
%ignore SenseItr::_CONSTRUCTOR;

%ignore TagsClass;

%include "SensoryData.h"
