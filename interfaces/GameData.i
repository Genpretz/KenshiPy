%{
#include "kenshi\GameData.h"
%}

%include "std_string.i"
%include "stdint.i"

// Suppress complex Boost/Ogre internal container members
%ignore GameData::ItemData;
%ignore GameData::ObjectInstance;
%ignore GameDataContainer::gamedataCatName;
%ignore GameDataContainer::gamedataID;
%ignore GameDataContainer::gamedataCatSID;
%ignore GameDataContainer::gamedataSID;
%ignore GameDataContainer::mainList;
%ignore GameDataContainer::killList;
%ignore GameDataContainer::_getAllData;

%ignore GameDataHeader::dependencies;
%ignore GameDataHeader::references;

// Methods using ogre_unordered_set - not exportable
%ignore GameData::getHandleList;
%ignore GameData::storeHandleList;
%ignore GameData::objectReferences;

// GameDataReference - constructors/destructor not exported from KenshiLib.lib
%ignore GameDataReference::GameDataReference;
%ignore GameDataReference::~GameDataReference;
%ignore GameDataReference::_CONSTRUCTOR;
%ignore GameDataReference::_DESTRUCTOR;
%ignore GameDataReference::operator=;
%ignore GameDataReference::getPtr;

%include "kenshi\GameData.h"
