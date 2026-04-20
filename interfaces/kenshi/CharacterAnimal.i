%{
#include "kenshi\CharacterAnimal.h"
%}

%include "std_string.i"
%include "stdint.i"

%ignore CharacterAnimal::itemInMouthTimeStamp;
%ignore CharacterAnimal::audioTimeStamp;
%ignore AnimalInventoryLayout::_NV_setupSections; // unresolved external symbol

%include "kenshi\CharacterAnimal.h"
