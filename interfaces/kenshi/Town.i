%{
#include "ArtifactItemData_stub.h"
#include "kenshi/Town.h"
%}

%include "std_string.i"

%import "kenshi/util/TagsClass.h"

// ArtifactItemData is only forward-declared in Town.h - full type unavailable
%ignore TownBase::artifacts;
%ignore TownBase::addArtifactItem;


// TagsClass<BuildingDesignation> - not wrappable (template, no copy constructor/operator=)
%ignore Town::facilitesWeHaveHere;
%ignore Town::_facilitesWeHaveHere;

//unexported symbols from headers
%ignore TownBase::DelayedSpawnMsg;
%ignore TownBase::delayedItemLoading;

%include "kenshi/Town.h"
