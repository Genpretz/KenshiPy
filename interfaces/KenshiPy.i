%module KenshiPy

// Tell SWIG to ignore __declspec and KLIB_EXPORT so it can parse the headers
#define KLIB_EXPORT
#define __declspec(x)
#define SWIG_init    PyInit_KenshiPy
#define SWIG_name    "KenshiPy"

%{
#include "ArtifactItemData_stub.h"
#include "CharMovement_guarded.h"

#include "Debug.h"
//#include "Defines.h"
//#include "stdafx.h"
//#include "Release_Assert.h"

#include "core/Functions.h"

#include "kenshi/util/Array2d.h"
#include "kenshi/util/hand.h"
#include "kenshi/util/iVector2.h"
#include "kenshi/util/lektor.h"
#include "kenshi/util/PerfTimer.h"
#include "kenshi/util/StringPair.h"
#include "kenshi/util/TagsClass.h"
#include "kenshi/util/UtilityT.h"

#include "kenshi/Bounty.h"
#include "kenshi/BountyManager.h"
#include "kenshi/Building.h"
#include "kenshi/Character.h"
#include "kenshi/CharacterAnimal.h"
#include "kenshi/CharacterHuman.h"
#include "kenshi/CharStats.h"
#include "kenshi/CombatClass.h"
#include "kenshi/CombatTechniqueData.h"
#include "kenshi/Damages.h"
#include "kenshi/Dialogue.h"
#include "kenshi/Faction.h"
#include "kenshi/FactionLeader.h"
#include "kenshi/FactionRelations.h"
#include "kenshi/FactionUniqueSquadManager.h"
#include "kenshi/FactionWarMgr.h"
#include "kenshi/FitnessSelector.h"
#include "kenshi/GameData.h"
#include "kenshi/GameDataManager.h"
#include "kenshi/GameSaveState.h"
#include "kenshi/GameWorld.h"
#include "kenshi/Gear.h"
#include "kenshi/GlobalConstants.h"
#include "kenshi/Globals.h"
#include "kenshi/InputHandler.h"
#include "kenshi/InstanceID.h"
#include "kenshi/Inventory.h"
#include "kenshi/Item.h"
#include "kenshi/Kenshi.h"
#include "kenshi/Logger.h"
#include "kenshi/MedicalSystem.h"
#include "kenshi/ModInfo.h"
#include "kenshi/OptionsHolder.h"
#include "kenshi/PhysicsActual.h"
#include "kenshi/PhysicsCollection.h"
#include "kenshi/PlayerInterface.h"
#include "kenshi/RaceData.h"
#include "kenshi/ResourceLoader.h"
#include "kenshi/RootObject.h"
#include "kenshi/RootObjectBase.h"
#include "kenshi/RootObjectFactory.h"
#include "kenshi/SaveFileSystem.h"
#include "kenshi/SaveInfo.h"
#include "kenshi/SaveManager.h"
#include "kenshi/SensoryData.h"
#include "kenshi/util/TagsClass.h"
#include "kenshi/Tasks.h"
#include "kenshi/ThreadClass.h"
#include "kenshi/TitleScreen.h"
#include "kenshi/Town.h"
#include "kenshi/TradeCulture.h"
#include "kenshi/WorldEventStateQuery.h"
%}

// Standard helpers
%include "std_string.i"
%include "stdint.i"

// Core KenshiLib interfaces (pre-existing)
%include "Debug.i"
%include "Functions.i"
%include "Kenshi.i"

//utilities
%include "util/Array2d.i"
%include "util/hand.i"
%include "util/iVector2.i"
%include "util/lektor.i"
%include "util/PerfTimer.i"
%include "util/StringPair.i"
%include "util/TagsClass.i"
%include "util/UtilityT.i"

// Game object interfaces
%include "Bounty.i"
%include "BountyManager.i"
%include "Damages.i"
%include "InstanceID.i"
%include "Logger.i"
%include "ThreadClass.i"
%include "GlobalConstants.i"
%include "OptionsHolder.i"
%include "SaveInfo.i"

// Data layer
%include "GameData.i"
%include "GameDataManager.i"
%include "GameSaveState.i"
%include "ModInfo.i"

// Physics / rendering
%include "PhysicsCollection.i"
%include "PhysicsActual.i"

// World / town
%include "RootObjectBase.i"
%include "RootObject.i"
%include "RootObjectFactory.i"
%include "Town.i"
%include "GameWorld.i"

// Faction system
%include "TradeCulture.i"
%include "FitnessSelector.i"
%include "FactionLeader.i"
%include "FactionRelations.i"
%include "FactionUniqueSquadManager.i"
%include "FactionWarMgr.i"
%include "Faction.i"

// Character system
%include "MedicalSystem.i"
%include "CharStats.i"
%include "CharMovement.i"
%include "CombatTechniqueData.i"
%include "CombatClass.i"
%include "Character.i"
%include "CharacterAnimal.i"
%include "CharacterHuman.i"

// Items / inventory
%include "Item.i"
%include "Gear.i"
%include "Inventory.i"

// Building
%include "Building.i"

// Dialogue / tasks
%include "Dialogue.i"
%include "Tasks.i"

// Player / input
%include "InputHandler.i"
%include "PlayerInterface.i"

// Race / sensory
%include "RaceData.i"
%include "SensoryData.i"

// Bounty / crime
%include "WorldEventStateQuery.i"

// Save system
%include "SaveFileSystem.i"
%include "SaveManager.i"

// Resource loading
%include "ResourceLoader.i"

// Globals (dllimport accessors)
%include "Globals.i"

//GUI
%include "TitleScreen.i"