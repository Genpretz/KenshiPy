# KenshiPy — Ignored & Renamed SWIG Symbols

This file documents every symbol suppressed with `%ignore` or renamed with
`%rename` across all `.i` interface files, along with the reason it was
excluded from the Python bindings.

Symbols listed here exist in the original Kenshi binary and are declared in
the KenshiLib headers, but are **not accessible from Python**. They are not
part of the KenshiPy project's own code — they are suppressions of upstream
Kenshi/KenshiLib symbols that cannot be safely or usefully wrapped.

---

## AppearanceManager.i

| Symbol | Reason |
|--------|--------|
| `AppearanceManager::APPEARANCE_IN_EDITOR_KEY` | Static `std::string` not exported from the Kenshi binary's import lib — linker error (LNK2001). |
| `AppearanceManager::APPEARANCE_FROM_FILE_KEY` | Static `std::string` not exported from the Kenshi binary's import lib — linker error (LNK2001). |
| `AppearanceManager::setMaterialTextures` | `protected` method; not callable externally. Also uses complex `boost::unordered_map` signature not exported from the import lib — linker error (LNK2001). |
| `AppearanceManager::getEditorData` | Extremely complex nested `boost::unordered_map` signature not exported from the import lib — linker error (LNK2001). Not useful from Python. |

---

## Bounty.i

| Symbol | Reason |
|--------|--------|
| `Bounty::bountyAssignmentStartedTime` | Internal timestamp using an unwrappable type. |

---

## BountyManager.i

| Symbol | Reason |
|--------|--------|
| `BountyManager::bounties` | Internal container type not wrappable by SWIG. |
| `BountyManager::crimeAgainst` | Internal container type not wrappable by SWIG. |
| `BountyManager::accessPassExpirationTime` | Internal timestamp type not wrappable by SWIG. |
| `BountyManager::prisonSentenceBeganTime` | Internal timestamp type not wrappable by SWIG. |

---

## Building.i

| Symbol | Reason |
|--------|--------|
| `Building::usageNodesIds` | Internal Ogre/engine container type. |
| `Building::usageNodes` | Internal Ogre/engine container type. |
| `Building::badNodes` | Internal Ogre/engine container type. |
| `Building::activeEffects` | Internal engine effect container. |
| `Building::effects` | Internal engine effect container. |
| `Building::AABB` | Ogre AABB type not wrapped. |
| `Building::infoWindow` | Internal MyGUI widget pointer. |
| `Building::physical` | Internal physics object pointer. |
| `Building::soundEmitter` | Internal audio engine pointer. |
| `Building::rootNode` | Internal Ogre scene node pointer. |
| `Building::buildingsManager` | Internal manager pointer. |
| `Building::myInterior` | Internal container type. |
| `Building::isFurnitureOf` | Internal container type. |
| `Building::baseMaterial` | Internal Ogre material pointer. |
| `Building::triggerVolume` | Internal physics trigger type. |
| `Building::getAABB` | Returns Ogre AABB type not wrapped. |
| `Building::setAABB` | Takes Ogre AABB type not wrapped. |
| `Building::getPartMaterial` | Returns Ogre material type not wrapped. |
| `Building::getBuildingPartMaterial` | Returns Ogre material type not wrapped. |
| `Building::setBuildingPartMaterial` | Takes Ogre material type not wrapped. |
| `Building::getLights` | Returns internal Ogre light container. |
| `Building::getPositionMarker` | Returns internal Ogre scene node. |
| `Building::getDirectionMarker` | Returns internal Ogre scene node. |
| `Building::getDirectionMarkerQuat` | Returns Ogre Quaternion; not wrapped. |
| `Building::calculateLeftPost` | Internal geometry calculation; Ogre types. |
| `Building::loadInstances` | Internal loading machinery; Ogre types. |
| `Building::loadParts` | Internal loading machinery; Ogre types. |
| `Building::loadPartEntity` | Internal loading machinery; Ogre entity type. |
| `Building::loadEntity` | Internal loading machinery; Ogre entity type. |
| `Building::loadEntityCallback` | Internal callback; Ogre types. |
| `Building::selectParts` | Internal selection machinery; Ogre types. |
| `Building::findPhysicalEntity` | Returns internal physics/Ogre entity type. |
| `Building::findPhysicalEntityInCollection` | Returns internal physics/Ogre entity type. |
| `Building::buildingContainsEntity` | Takes internal Ogre entity pointer. |
| `Building::getEntityMaterialName` | Takes internal Ogre entity pointer. |
| `Building::setPartVisible` | Takes internal Ogre entity pointer. |
| `Building::restoreMaterialToPhysical` | Takes internal Ogre entity/material types. |
| `Building::getRootNode` | Returns internal Ogre scene node. |
| `Building::positionMarker` | Internal Ogre scene node member. |
| `Building::refreshGUIChangesFlag` | Internal GUI state flag; not useful from Python. |

---

## CharMovement.i

| Symbol | Reason |
|--------|--------|
| `physHit::shape` | Internal PhysX shape pointer; not wrapped. |
| `physHit::hitObject` | Internal PhysX object pointer; not wrapped. |
| `physHit::_hitObjectUnsafePtr` | Raw unsafe pointer; not safe to expose. |
| `physHit::normal` | Ogre Vector3; not wrapped in this context. |
| `physHit::position` | Ogre Vector3; not wrapped in this context. |
| `SpeedGroup::members` | Internal container type not wrappable by SWIG. |
| `SpeedGroup::position` | Ogre Vector3 member on internal struct. |
| `SpeedGroup::direction` | Ogre Vector3 member on internal struct. |

---

## CharStats.i

| Symbol | Reason |
|--------|--------|
| `CharStats::_weatherProtections` | Internal container using unwrappable Ogre/engine types. |
| `CharStats::getTotalAttackDamageFor` | Also ignored in `Damages.i`; uses internal type. |
| `CharStats::attacks` | Internal container type not wrappable by SWIG. |
| `CharStats::blocks` | Internal container type not wrappable by SWIG. |

---

## Character.i

### Renamed symbols (enum value collision avoidance)

| Original | Renamed to | Reason |
|----------|-----------|--------|
| `RagdollPart::NONE` | `RAGDOLL_NONE` | Conflicts with other `NONE` enum values at global Python scope. |
| `RagdollPart::WHOLE` | `RAGDOLL_WHOLE` | Namespacing for clarity. |
| `RagdollPart::RIGHT_ARM` | `RAGDOLL_RIGHT_ARM` | Conflicts with `RobotLimbs::Limb::RIGHT_ARM`. |
| `RagdollPart::LEFT_ARM` | `RAGDOLL_LEFT_ARM` | Conflicts with `RobotLimbs::Limb::LEFT_ARM`. |
| `RagdollPart::HEAD` | `RAGDOLL_HEAD` | Conflicts with `itemType::HEAD`. |
| `RagdollPart::RIGHT_LEG` | `RAGDOLL_RIGHT_LEG` | Conflicts with `RobotLimbs::Limb::RIGHT_LEG`. |
| `RagdollPart::LEFT_LEG` | `RAGDOLL_LEFT_LEG` | Conflicts with `RobotLimbs::Limb::LEFT_LEG`. |
| `RagdollPart::CARRY_MODE` | `RAGDOLL_CARRY_MODE` | Namespacing for clarity. |
| `RagdollPart::ARMS` | `RAGDOLL_ARMS` | Namespacing for clarity. |
| `RagdollPart::LEGS` | `RAGDOLL_LEGS` | Namespacing for clarity. |
| `RagdollPart::ALL` | `RAGDOLL_ALL` | Namespacing for clarity. |

### Ignored members

| Symbol | Reason |
|--------|--------|
| `Character::ragdollMessages` | Internal container using unwrappable internal message types. |
| `Character::activeEffects` | Internal engine effect container. |
| `Character::particleEffects` | Internal Ogre particle system container. |
| `Character::inWhat` | Internal container type not wrappable by SWIG. |
| `Character::slaveOwner` | Internal container type not wrappable by SWIG. |
| `Character::carryingObject` | Internal container type not wrappable by SWIG. |
| `Character::messageSubject` | Internal container type not wrappable by SWIG. |
| `Character::_destinationInsideBuilding` | Internal container type not wrappable by SWIG. |
| `Character::msgCarryMode` | Internal message container type. |
| `Character::audioData` | Internal audio engine pointer. |
| `Character::audioObject` | Internal audio engine pointer. |
| `Character::audioEmitter` | Internal audio engine pointer. |
| `Character::getBoneWorldPosition` | Returns Ogre Vector3 via internal bone/skeleton types. |
| `Character::getPredictedPosition` | Returns Ogre Vector3 via internal navigation types. |
| `Character::getAllAttackers` | Fills a `lektor` container; `lektor` output parameters not wrappable. |
| `Character::ragdollNavmeshPosition` | Internal navmesh/Ogre position type. |
| `Character::RagdollMsg` | Nested struct with no copy constructor available. |
| `Character::CarryMsg` | Nested struct with no copy constructor available. |
| `Character::CharMessage` | Nested struct with no copy constructor available. |
| `Character::WhoSeesMe` | No copy constructor present in the binary (`no_addr` in RE header); SWIG cannot generate wrapper. |
| `Character::whoSeesMeSneaking` | Member of ignored type `WhoSeesMe`. |

---

## CharacterAnimal.i

| Symbol | Reason |
|--------|--------|
| `CharacterAnimal::itemInMouthTimeStamp` | Internal timestamp type not wrappable by SWIG. |
| `CharacterAnimal::audioTimeStamp` | Internal timestamp type not wrappable by SWIG. |

---

## CombatClass.i

| Symbol | Reason |
|--------|--------|
| `ParticlePool::particles` | Internal Ogre particle container. |
| `ParticlePool::ParticleData` | Internal nested struct using Ogre types. |
| `ParticlePool::addParticle` | Takes internal Ogre particle types. |
| `ParticlePool::shiftParticles` | Internal particle management; Ogre types. |
| `CombatClass::effectsToAdd` | Internal container using unwrappable effect types. |
| `CombatClass::effectsPool` | Internal `ParticlePool`; pool type not fully wrapped. |

---

## CombatTechniqueData.i

| Symbol | Reason |
|--------|--------|
| `CombatTechniqueData::events` | Internal container using unwrappable event types. |
| `CombatTechniqueData::impactPoints` | Internal container using unwrappable Ogre/physics types. |

---

## Damages.i

| Symbol | Reason |
|--------|--------|
| `Damages::Damages` | Constructor uses internal types; wrapped via other means. |
| `Damages::~Damages` | Destructor; managed by SWIG automatically. |
| `Damages::_CONSTRUCTOR` | KenshiLib RE artifact; not a real public API. |
| `Damages::_DESTRUCTOR` | KenshiLib RE artifact; not a real public API. |
| `CharStats::getTotalAttackDamageFor` | Uses internal damage container types; also ignored in CharStats.i. |

---

## Dialogue.i

| Symbol | Reason |
|--------|--------|
| `Dialogue` | Entire class ignored; uses deeply nested internal dialogue tree types not wrappable by SWIG. |

---

## Enums.i

### Renamed symbols (enum value collision avoidance)

| Original | Renamed to | Reason |
|----------|-----------|--------|
| `EffectType::NONE` | `EFFECT_NONE` | Conflicts with other `NONE` values at global Python scope. |
| `EffectType::CAMERA` | `EFFECT_CAMERA` | Namespacing for clarity. |
| `EffectType::POINT` | `EFFECT_POINT` | Namespacing for clarity. |
| `EffectType::WANDERING` | `EFFECT_WANDERING` | Namespacing for clarity. |
| `EffectType::GLOBAL` | `EFFECT_GLOBAL` | Namespacing for clarity. |
| `EffectType::CAMERA_RAIN` | `EFFECT_CAMERA_RAIN` | Namespacing for clarity. |
| `EffectType::CAMERA_ACID_RAIN` | `EFFECT_CAMERA_ACID_RAIN` | Namespacing for clarity. |
| `EffectType::POINT_LIGHTING` | `EFFECT_POINT_LIGHTING` | Namespacing for clarity. |
| `EffectType::WANDERING_STORM` | `EFFECT_WANDERING_STORM` | Namespacing for clarity. |
| `EffectType::WANDERING_GAS` | `EFFECT_WANDERING_GAS` | Namespacing for clarity. |
| `EffectType::GLOBAL_POINT` | `EFFECT_GLOBAL_POINT` | Namespacing for clarity. |

---

## Faction.i

| Symbol | Reason |
|--------|--------|
| `Faction::BuildingSwaps` | Internal nested struct using unwrappable container types. |
| `Faction::CharacteristicsData` | Internal nested struct using unwrappable container types. |
| `Faction::ranks` | Internal container type not wrappable by SWIG. |
| `Faction::createNewEmptyUnloadedPlatoon` | Returns internal platoon type via pointer-out pattern. |
| `Faction::createNewEmptyActivePlatoon` | Returns internal platoon type via pointer-out pattern. |
| `Faction::createPlatoonUnloaded` | Internal platoon factory; uses unwrappable types. |
| `Faction::createPlatoonAuto` | Internal platoon factory; uses unwrappable types. |
| `Faction::createPlatoonsAuto` | Internal platoon factory; uses unwrappable types. |
| `Faction::addListMuto` | Takes internal mutex-protected list type. |
| `FactionManager::addListMuto` | Takes internal mutex-protected list type. |

---

## FactionLeader.i

| Symbol | Reason |
|--------|--------|
| `FactionLeader::biomeTerritory` | Internal Ogre/engine biome container type. |

---

## FactionUniqueSquadManager.i

| Symbol | Reason |
|--------|--------|
| `FactionUniqueSquadManager::squads` | Internal container type not wrappable by SWIG. |
| `FactionUniqueSquadManager::UniqueSpawnData::existingSquadsList` | Internal container type not wrappable by SWIG. |

---

## FactionWarMgr.i

| Symbol | Reason |
|--------|--------|
| `FactionWarMgr::forces` | Internal container type not wrappable by SWIG. |
| `FactionWarMgr::hiredForces` | Internal container type not wrappable by SWIG. |
| `FactionWarMgr::activeCampaigns` | Internal container type not wrappable by SWIG. |
| `FactionWarMgr::myTowns` | Internal container type not wrappable by SWIG. |
| `FactionWarMgr::biomeTerritories` | Internal Ogre/engine biome container type. |
| `FactionWarMgr::biomeNoGoZones` | Internal Ogre/engine biome container type. |
| `FactionWarMgr::nextUpdateTime` | Internal timestamp type not wrappable by SWIG. |
| `FactionWarMgr::triggerCampaign` | Takes internal campaign/container types. |

---

## GameData.i

| Symbol | Reason |
|--------|--------|
| `GameData::ItemData` | Internal nested struct using unwrappable container types. |
| `GameData::ObjectInstance` | Internal nested struct using unwrappable container types. |
| `GameDataContainer::gamedataCatName` | Internal container identity field; not useful from Python. |
| `GameDataContainer::gamedataID` | Internal container identity field; not useful from Python. |
| `GameDataContainer::gamedataCatSID` | Internal container identity field; not useful from Python. |
| `GameDataContainer::gamedataSID` | Internal container identity field; not useful from Python. |
| `GameDataContainer::mainList` | Internal container type not wrappable by SWIG. |
| `GameDataContainer::killList` | Internal container type not wrappable by SWIG. |
| `GameDataContainer::_getAllData` | Returns internal container type. |
| `GameDataHeader::dependencies` | Internal container type not wrappable by SWIG. |
| `GameDataHeader::references` | Internal container type not wrappable by SWIG. |
| `GameData::getHandleList` | Returns internal handle list container. |
| `GameData::storeHandleList` | Takes internal handle list container. |
| `GameData::objectReferences` | Internal reference container not wrappable by SWIG. |
| `GameDataReference::GameDataReference` | Constructor uses internal ownership semantics incompatible with SWIG. |
| `GameDataReference::~GameDataReference` | Destructor; ownership managed internally. |
| `GameDataReference::_CONSTRUCTOR` | KenshiLib RE artifact; not a real public API. |
| `GameDataReference::_DESTRUCTOR` | KenshiLib RE artifact; not a real public API. |
| `GameDataReference::operator=` | Assignment operator; ownership semantics incompatible with SWIG. |
| `GameDataReference::getPtr` | Returns raw internal pointer; unsafe to expose directly. |

---

## GameDataManager.i

| Symbol | Reason |
|--------|--------|
| `GameDataManager::getBuildings` | Returns internal building container type not wrappable by SWIG. |

---

## GameLauncher.i

| Symbol | Reason |
|--------|--------|
| `CExCheckListBox` | Win32 MFC/dialog control class; not relevant to Python scripting. |
| `GameLauncher` | Win32 launcher dialog class; not relevant to Python scripting. |

---

## GameSaveState.i

| Symbol | Reason |
|--------|--------|
| `GameSaveState::states` | Internal container type not wrappable by SWIG. |
| `GameSaveState::getAllStates` | Returns internal container type. |
| `GameSaveState::pos` | Internal Ogre Vector3 member on save state struct. |
| `GameSaveState::rot` | Internal Ogre Quaternion member on save state struct. |

---

## GameWorld.i

| Symbol | Reason |
|--------|--------|
| `GameWorld::tempSpawnsDisableTimer` | Internal timer type not wrappable by SWIG. |
| `SimpleTimeStamper::timer` | Internal timer type not wrappable by SWIG. |

---

## Inventory.i

| Symbol | Reason |
|--------|--------|
| `InventorySection::SectionItem` | Internal nested struct using unwrappable container types. |

---

## Item.i

| Symbol | Reason |
|--------|--------|
| `InventoryItemBase::inventoryPos` | Internal position type not wrappable by SWIG. |
| `Item::stolenFrom` | Internal container type not wrappable by SWIG. |

---

## MedicalSystem.i

### Renamed symbols

| Original | Renamed to | Reason |
|----------|-----------|--------|
| `AttackDirection::FRONT` | `ATTACK_DIR_FRONT` | Namespacing to avoid collision with other direction enums. |
| `AttackDirection::BACK` | `ATTACK_DIR_BACK` | Namespacing for clarity. |
| `AttackDirection::LEFT` | `ATTACK_DIR_LEFT` | Namespacing for clarity. |
| `AttackDirection::RIGHT` | `ATTACK_DIR_RIGHT` | Namespacing for clarity. |
| `AttackDirection::TOP` | `ATTACK_DIR_TOP` | Namespacing for clarity. |
| `AttackDirection::BOTTOM` | `ATTACK_DIR_BOTTOM` | Namespacing for clarity. |

### Ignored symbols

| Symbol | Reason |
|--------|--------|
| `MedicalSystem` | Entire class ignored; deeply nested internal medical simulation types not wrappable. Also ignored as a `Character` member in `Character.i`. |
| `RobotLimbs::states` | Internal container type not wrappable by SWIG. |
| `RobotLimbs::items` | Internal container type not wrappable by SWIG. |

---

## OptionsHolder.i

| Symbol | Reason |
|--------|--------|
| `OptionsHolder::compositors` | Internal Ogre compositor container type not wrappable by SWIG. |

---

## PhysicsActual.i

| Symbol | Reason |
|--------|--------|
| `NxUserTriggerReport` | PhysX abstract callback class; not useful from Python. |

---

## PhysicsCollection.i

| Symbol | Reason |
|--------|--------|
| `PhysicalEntity::ent` | Internal Ogre entity pointer; Ogre entity not wrapped. |
| `PhysicalEntity::parent` | Internal scene node pointer; Ogre types not wrapped. |
| `WeatherRegion` | Internal engine weather region class; uses unwrappable Ogre/engine types. |

---

## PlayerInterface.i

| Symbol | Reason |
|--------|--------|
| `SelectionBox::volume` | Internal Ogre/physics volume type not wrapped. |
| `SelectionBox::widget` | Internal MyGUI widget pointer. |
| `SelectionBox::contains` | Takes internal Ogre/physics types. |
| `ContextMenu::orders` | Internal container type not wrappable by SWIG. |
| `ContextMenu::menuGUI` | Internal MyGUI widget pointer. |
| `ContextMenu::menuGUI2` | Internal MyGUI widget pointer. |

---

## RaceData.i

| Symbol | Reason |
|--------|--------|
| `RaceData::specialFoods` | Internal container type not wrappable by SWIG. |
| `RaceData::AllRaces` | Internal static container type not wrappable by SWIG. |
| `RaceData::statMods` | Internal container type not wrappable by SWIG. |
| `RaceData::weatherImmunities` | Internal container type not wrappable by SWIG. |
| `RaceData::bloodColour` | Internal Ogre colour type not wrapped. |

---

## ResourceLoader.i

| Symbol | Reason |
|--------|--------|
| `ResourceLoader::TextureLoadData` | Internal nested struct using Ogre texture types. |
| `ResourceLoader::MeshLoadData` | Internal nested struct using Ogre mesh types. |
| `ResourceLoader::ResourceLoadRequestMesh` | Internal request struct using Ogre types. |
| `ResourceLoader::ResourceLoadRequestTexture` | Internal request struct using Ogre types. |
| `ResourceLoader::loadingMeshQueueMutex` | Internal Boost mutex member; not wrappable. |
| `ResourceLoader::texturesLoadingMutex` | Internal Boost mutex member; not wrappable. |
| `ResourceLoader::texturesLoadedMutex` | Internal Boost mutex member; not wrappable. |
| `ResourceLoader::loadModelEntity` | Returns internal Ogre entity type. |
| `ResourceLoader::loadModelEntityDetached` | Returns internal Ogre entity type. |
| `ResourceLoader::loadTextureUnitArray` | Takes internal Ogre texture unit types. |
| `ResourceLoader::EMPTY_CALLBACK` | Internal static callback object; not useful from Python. |
| `ResourceLoader::MeshLodStrategy` | Internal Ogre LOD strategy type. |

---

## RootObjectBase.i

| Symbol | Reason |
|--------|--------|
| `RootObjectBase::pos` | Internal Ogre Vector3 member; position exposed via `getPosition()` instead. |
| `RootObjectBase::handle` | Raw internal handle member; handle access exposed via `hand` wrapper instead. |

---

## RootObjectFactory.i

| Symbol | Reason |
|--------|--------|
| `RootObjectFactory::todoMutex` | Internal Boost mutex member; not wrappable. |

---

## SaveFileSystem.i

| Symbol | Reason |
|--------|--------|
| `SaveFileSystem::fileSystem` | Internal filesystem container type not wrappable by SWIG. |
| `SaveFileSystem::deletedFiles` | Internal container type not wrappable by SWIG. |
| `SaveFileSystem::paths` | Internal container type not wrappable by SWIG. |
| `SaveFileSystem::oldCurrentFolders` | Internal container type not wrappable by SWIG. |
| `SaveFileSystem::currentMutex` | Internal Boost mutex member; not wrappable. |
| `SaveFileSystem::saveLog` | Internal container type not wrappable by SWIG. |

---

## SaveInfo.i

| Symbol | Reason |
|--------|--------|
| `SaveInfo::SaveInfo` | Constructor uses internal serialisation types incompatible with SWIG. |
| `SaveInfo::_CONSTRUCTOR` | KenshiLib RE artifact; not a real public API. |

---

## SaveManager.i

| Symbol | Reason |
|--------|--------|
| `SaveManager::scanGames` | Returns internal container type not wrappable by SWIG. |

---

## SensoryData.i

| Symbol | Reason |
|--------|--------|
| `SeenSomeone::lastSeenTime` | Internal timestamp type not wrappable by SWIG. |
| `SeenSomeone::lastPosition` | Internal Ogre Vector3 member on internal struct. |
| `SeenSomeone::type` | Internal enum type not wrappable in this context. |
| `SenseItr::SenseItr` | Constructor takes internal iterator types; not constructible from Python. |
| `SenseItr::_CONSTRUCTOR` | KenshiLib RE artifact; not a real public API. |
| `TagsClass` | Entire class ignored; internal bitmask/tag system not useful from Python. |

---

## ThreadClass.i

| Symbol | Reason |
|--------|--------|
| `ThreadClass::runMute` | Internal Boost mutex member; not wrappable. |
| `ThreadClass::lockedWhileRunningMute` | Internal Boost mutex member; not wrappable. |

---

## Town.i

| Symbol | Reason |
|--------|--------|
| `TownBase::artifacts` | Internal container type not wrappable by SWIG. |
| `TownBase::addArtifactItem` | Takes internal artifact container type. |
| `Town::facilitesWeHaveHere` | Internal container type not wrappable by SWIG. |
| `Town::_facilitesWeHaveHere` | Internal container type not wrappable by SWIG. |

---

## TradeCulture.i

| Symbol | Reason |
|--------|--------|
| `TradeCulture::forbiddenItems` | Internal container type not wrappable by SWIG. |
| `TradeCulture::illegalBuildings` | Internal container type not wrappable by SWIG. |
| `TradeCulture::happyBuildings` | Internal container type not wrappable by SWIG. |
| `TradeCulture::illegalItems` | Internal container type not wrappable by SWIG. |
| `TradeCulture::tradeGoodsMults` | Internal container type not wrappable by SWIG. |
| `TradeCulture::getForbiddenItemsList` | Returns internal container type. |
| `TradeCulture::getAllIllegalItems` | Returns internal container type. |

---

## WorldEventStateQuery.i

| Symbol | Reason |
|--------|--------|
| `WorldEventStateQuery::statesData` | Internal container type not wrappable by SWIG. |
| `WorldEventStateQuery::uniqueNPCsAre` | Internal container type not wrappable by SWIG. |
| `WorldEventStateQuery::uniqueNPCsAreNot` | Internal container type not wrappable by SWIG. |
| `WorldEventStateQuery::towns` | Internal container type not wrappable by SWIG. |
| `WorldEventStateQuery::isAllyOf` | Internal container type not wrappable by SWIG. |
| `WorldEventStateQuery::isEnemyOf` | Internal container type not wrappable by SWIG. |
| `WorldEventStateQueryList::statesList` | Internal container type not wrappable by SWIG. |

---

## hand.i

| Symbol | Reason |
|--------|--------|
| `hand::NULL_HAND` | Static sentinel value; direct construction from Python not safe. |
| `hand::_CONSTRUCTOR` | KenshiLib RE artifact; not a real public API. |
| `hand::_DESTRUCTOR` | KenshiLib RE artifact; not a real public API. |
| `hand::operator=(RootObjectBase* a)` | Assignment from raw pointer; ownership semantics unsafe from Python. |
| `hand::operator=(const int& a)` | Assignment from raw integer handle; not safe to expose. |
| `hand::operator==(bool a) const` | Ambiguous bool comparison operator; conflicts with SWIG bool handling. |
| `hand::_NV_operator_equal(bool a) const` | KenshiLib RE artifact duplicate of ignored operator. |
| `hand::_NV_operator_equal(const hand& a) const` | KenshiLib RE artifact; not a real public API. |
| `hand::_NV_operator_notequal(const hand& a) const` | KenshiLib RE artifact; not a real public API. |

### Renamed symbols

| Original | Renamed to | Reason |
|----------|-----------|--------|
| `hand::operator bool() const` | `__bool__` | Maps C++ bool conversion to Python's `__bool__` protocol so `if handle:` works naturally. |
