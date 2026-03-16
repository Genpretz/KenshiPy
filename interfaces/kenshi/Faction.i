%{
#include "kenshi\Faction.h"
%}

%include "std_string.i"
%include "stdint.i"

// Suppress members using Boost thread, internal containers, and complex types
%ignore Faction::BuildingSwaps;
%ignore Faction::CharacteristicsData;
%ignore Faction::ranks;
%ignore Faction::createNewEmptyUnloadedPlatoon;
%ignore Faction::createNewEmptyActivePlatoon;
%ignore Faction::createPlatoonUnloaded;
%ignore Faction::createPlatoonAuto;
%ignore Faction::createPlatoonsAuto;

// Suppress boost::shared_mutex and related internal members
%ignore Faction::addListMuto;
%ignore FactionManager::addListMuto;
%nocopyctor Faction;
%nocopyctor FactionManager;

%include "kenshi\Faction.h"
