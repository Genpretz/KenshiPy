%module KenshiPy

#define SWIG_init    PyInit_KenshiPy
#define SWIG_name    "KenshiPy"

%{
#include "CharMovement_guarded.h"

#include "Debug.h"

#include "kenshi/util/Array2d.h"
#include "kenshi/util/hand.h"
#include "kenshi/util/iVector2.h"
#include "kenshi/util/lektor.h"
#include "kenshi/util/PerfTimer.h"
#include "kenshi/util/StringPair.h"
#include "kenshi/util/TagsClass.h"
#include "kenshi/util/UtilityT.h"


#include "kenshi/Appearance.h"
#include "kenshi/Bounty.h"
#include "kenshi/BountyManager.h"
#include "kenshi/Building/Building.h"
#include "kenshi/Character.h"
#include "kenshi/CharacterAnimal.h"
#include "kenshi/CharacterHuman.h"
#include "kenshi/CharStats.h"
#include "kenshi/CombatClass.h"
#include "kenshi/CombatTechniqueData.h"
#include "kenshi/Damages.h"
#include "kenshi/Dialogue.h"
#include "kenshi/Enums.h"
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
#include "kenshi/Tasker.h"
#include "kenshi/ThreadClass.h"
#include "kenshi/gui/TitleScreen.h"
#include "kenshi/Town.h"
#include "kenshi/TradeCulture.h"
#include "kenshi/WorldEventStateQuery.h"
%}

// Standard helpers
%include "std_string.i"
%include "stdint.i"

// Core KenshiLib interfaces (pre-existing)
%include "Debug.i"
%include "kenshi/Kenshi.i"
%include "kenshi/GameWorld.i"

//utilities
%include "kenshi/util/Array2d.i"
%include "kenshi/util/hand.i"
%include "kenshi/util/iVector2.i"
%include "kenshi/util/lektor.i"
%include "kenshi/util/PerfTimer.i"
%include "kenshi/util/StringPair.i"
%include "kenshi/util/TagsClass.i"
%include "kenshi/util/UtilityT.i"


%include "kenshi/Bounty.i"
%include "kenshi/BountyManager.i"
%include "kenshi/Damages.i"
%include "kenshi/InstanceID.i"
%include "kenshi/Logger.i"
%include "kenshi/ThreadClass.i"
%include "kenshi/GlobalConstants.i"
%include "kenshi/OptionsHolder.i"
%include "kenshi/SaveInfo.i"
%include "kenshi/Enums.i"
%include "kenshi/ResourceLoader.i"
%include "kenshi/Globals.i"
%include "kenshi/gui/TitleScreen.i"

// Data layer
%include "kenshi/GameData.i"
%include "kenshi/GameDataManager.i"
%include "kenshi/GameSaveState.i"
%include "kenshi/ModInfo.i"

// Physics / rendering
%include "kenshi/PhysicsCollection.i"
%include "kenshi/PhysicsActual.i"

// World / town
%include "kenshi/RootObjectBase.i"
%include "kenshi/RootObject.i"
%include "kenshi/RootObjectFactory.i"
%include "kenshi/Town.i"


// Faction system
%include "kenshi/TradeCulture.i"
%include "kenshi/FitnessSelector.i"
%include "kenshi/FactionLeader.i"
%include "kenshi/FactionRelations.i"
%include "kenshi/FactionUniqueSquadManager.i"
%include "kenshi/FactionWarMgr.i"
%include "kenshi/Faction.i"

// Character system
%include "kenshi/MedicalSystem.i"
%include "kenshi/CharStats.i"
%include "kenshi/CharMovement.i"
%include "kenshi/CombatTechniqueData.i"
%include "kenshi/CombatClass.i"
%include "kenshi/Character.i"
%include "kenshi/CharacterAnimal.i"
%include "kenshi/CharacterHuman.i"

// Items / inventory
%include "kenshi/Item.i"
%include "kenshi/Gear.i"
%include "kenshi/Inventory.i"

// Building
%include "kenshi/Building.i"

// Dialogue / tasks
%include "kenshi/Dialogue.i"
%include "kenshi/Tasker.i"

// Player / input
%include "kenshi/InputHandler.i"
%include "kenshi/PlayerInterface.i"

%include "kenshi/Platoon.i"
%include "kenshi/Appearance.i"
%include "kenshi/AppearanceManager.i"

// Race / sensory
%include "kenshi/RaceData.i"
%include "kenshi/SensoryData.i"

// Bounty / crime
%include "kenshi/WorldEventStateQuery.i"

// Save system
%include "kenshi/SaveFileSystem.i"
%include "kenshi/SaveManager.i"

//MyGUI
%include "mygui/MyGUI.i"

//Ogre
%include "ogre/Ogre.i"

// Python callbacks
%{
#include "Callbacks.h"
%}
void RegisterKeyDownCallback(PyObject* callable);
void RegisterFrameCallback(PyObject* callable);
void UnregisterKeyDownCallback(PyObject* callable);
void UnregisterFrameCallback(PyObject* callable);
