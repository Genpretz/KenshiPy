%{
#include "CharStats.h"
%}

%include "std_string.i"
%include "stdint.i"

// Suppress internal container types
%ignore CharStats::_weatherProtections;
// Returns Damages by value which cannot be copied (private Ogre allocator)
%ignore CharStats::getTotalAttackDamageFor;
// Static members not exported from KenshiLib.lib
%ignore CharStats::attacks;
%ignore CharStats::blocks;

%include "CharStats.h"
