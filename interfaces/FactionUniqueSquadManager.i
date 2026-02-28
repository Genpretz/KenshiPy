%{
#include "FactionUniqueSquadManager.h"
%}

%include "std_string.i"

%ignore FactionUniqueSquadManager::squads;
%ignore FactionUniqueSquadManager::UniqueSpawnData::existingSquadsList;

%include "FactionUniqueSquadManager.h"
