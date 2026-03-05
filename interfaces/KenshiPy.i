%module KenshiPy

// Tell SWIG to ignore __declspec and KLIB_EXPORT so it can parse the headers
#define KLIB_EXPORT
#define __declspec(x)
#define SWIG_init    PyInit_KenshiPy
#define SWIG_name    "KenshiPy"

%{
//#include "ArtifactItemData_stub.h"
#include "hand.h"
#include "Bounty.h"
#include "BountyManager.h"
#include "Building.h"
#include "Character.h"
#include "CharacterAnimal.h"
#include "CharacterHuman.h"
//#include "CharMovement_guarded.h"
#include "CharStats.h"
#include "CombatClass.h"
#include "CombatTechniqueData.h"
#include "Damages.h"
#include "Dialogue.h"
#include "Debug.h"
#include "Faction.h"
#include "FactionLeader.h"
#include "FactionRelations.h"
#include "FactionUniqueSquadManager.h"
#include "FactionWarMgr.h"
#include "FitnessSelector.h"
#include "Functions.h"
#include "GameData.h"
#include "GameDataManager.h"
#include "GameSaveState.h"
#include "GameWorld.h"
#include "Gear.h"
#include "GlobalConstants.h"
#include "Globals.h"
#include "InputHandler.h"
#include "InstanceID.h"
#include "Inventory.h"
#include "Item.h"
#include "Kenshi.h"
#include "Logger.h"
#include "MedicalSystem.h"
#include "ModInfo.h"
#include "OptionsHolder.h"
#include "PhysicsActual.h"
#include "PhysicsCollection.h"
#include "PlayerInterface.h"
#include "RaceData.h"
#include "ResourceLoader.h"
#include "RootObject.h"
#include "RootObjectBase.h"
#include "RootObjectFactory.h"
#include "SaveFileSystem.h"
#include "SaveInfo.h"
#include "SaveManager.h"
#include "SensoryData.h"
#include "TagsClass.h"
#include "Tasks.h"
#include "ThreadClass.h"
#include "TitleScreen.h"
#include "Town.h"
#include "TradeCulture.h"
#include "WorldEventStateQuery.h"
%}

// Standard helpers
%include "std_string.i"
%include "stdint.i"

// Core KenshiLib interfaces (pre-existing)
%include "Debug.i"
%include "Functions.i"
%include "Kenshi.i"

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
%include "hand.i"
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

//Utilities
%include "TagsClass.i"

//GUI
%include "TitleScreen.i"