%{
#include "ArtifactItemData_stub.h"
#include "Town.h"
%}

%include "std_string.i"

// ArtifactItemData is only forward-declared in Town.h - full type unavailable
%ignore TownBase::artifacts;
%ignore TownBase::addArtifactItem;
// Static member not exported from KenshiLib.lib
%ignore TownBase::delayedItemLoading;
%ignore TownBase::findAllBuildingsOfType;
%ignore TownBase::_NV_findAllBuildingsOfType;
%ignore TownBase::findAllBuildingsWithFunction;
%ignore TownBase::_NV_findAllBuildingsWithFunction;

%include "Town.h"
