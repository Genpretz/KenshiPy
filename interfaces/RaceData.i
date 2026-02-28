%{
#include "RaceData.h"
%}

%include "std_string.i"

%ignore RaceData::specialFoods;
%ignore RaceData::AllRaces;
%ignore RaceData::statMods;
%ignore RaceData::weatherImmunities;
%ignore RaceData::bloodColour;

%include "RaceData.h"
