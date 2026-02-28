%{
#include "CharacterAnimal.h"
%}

%include "std_string.i"
%include "stdint.i"

%ignore CharacterAnimal::itemInMouthTimeStamp;
%ignore CharacterAnimal::audioTimeStamp;

%include "CharacterAnimal.h"
