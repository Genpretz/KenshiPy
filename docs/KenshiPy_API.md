# KenshiPy API Reference

KenshiPy is a SWIG-generated Python extension module (`_KenshiPy`) that exposes KenshiLib's C++ game object bindings to Python scripts loaded by Kenshi mods. It is built from `KenshiPy.i` and the associated per-class `.i` files.

---

## Entry Points (Global Accessor Functions)

These are the primary way to reach live game state from Python. Defined inline in `Globals.i`.

| Function | Return Type | Description |
|---|---|---|
| `getGameWorld()` | `GameWorld*` | Returns the global game world instance (`ou`). |
| `getGlobalConstants()` | `GlobalConstants*` | Returns the global constants object (`con`). |
| `getInputHandler()` | `InputHandler*` | Returns the input handler (`key`). |
| `getOptionsHolder()` | `OptionsHolder*` | Returns the options/settings object (`options`). |

Two free functions are also declared in `Globals.i`:

| Function | Signature | Description |
|---|---|---|
| `showErrorMessage` | `void showErrorMessage()` | Triggers the game's error message display. |
| `modMedicalSkill` | `float modMedicalSkill(float skill, Item* equipment, float frameTIME)` | Applies equipment modifiers to a raw medical skill value. |

---

## KenshiLib Namespace (Core)

### Debug (`Debug.h`)

Free functions for logging.

| Function | Signature | Description |
|---|---|---|
| `DebugLog` | `void DebugLog(const std::string&)` | Appends a message to the debug log. |
| `DebugLog` | `void DebugLog(const char*)` | Overload accepting a C string. |
| `ErrorLog` | `void ErrorLog(const std::string&)` | Appends a message to the error log. |
| `ErrorLog` | `void ErrorLog(const char*)` | Overload accepting a C string. |
| `GetDebugLog` | `std::string GetDebugLog()` | Returns the accumulated debug log as a string. |
| `GetLastErrorStdStr` | `std::string GetLastErrorStdStr()` | Returns the result of `WINAPI GetLastError()` as a string. |

### Kenshi (`Kenshi.h`) — `KenshiLib` namespace

#### `BinaryVersion` class

Identifies which build/platform of Kenshi is running.

| Member | Type | Description |
|---|---|---|
| `GetPlatform()` | `KenshiPlatform` | Returns the platform enum (GOG, STEAM, UNKNOWN). |
| `GetPlatformStr()` | `std::string` | Returns `"GOG"`, `"Steam"`, or `"Unknown"`. |
| `GetBinaryName()` | `std::string` | Returns the exe filename for the platform. |
| `GetVersion()` | `std::string` | Returns the version string. |
| `ToString()` | `std::string` | Returns `"<version> <platform>"`. |

#### Enum: `KenshiLib::BinaryVersion::KenshiPlatform`
`GOG`, `STEAM`, `UNKNOWN`

#### Free function

| Function | Signature | Description |
|---|---|---|
| `GetKenshiVersion` | `BinaryVersion GetKenshiVersion()` | Returns the detected version/platform of the running Kenshi binary. |

### Functions (`Functions.h`) — `KenshiLib` namespace

Hooking utilities. Not generally called from mod scripts directly.

| Function | Description |
|---|---|
| `GetRealAddress(void*)` | Returns the resolved runtime address of a function pointer. |
| `AddHook(void*, void*, void**)` | Installs a detour hook via multihook. Returns `HookStatus`. |

#### Enum: `KenshiLib::HookStatus`
`SUCCESS`, `FAIL`

---

## Game World

### `GameWorld` class

The central game simulation object, retrieved via `getGameWorld()`.

**Key data members (accessible as attributes):**

| Member | Type | Description |
|---|---|---|
| `initialized` | `bool` | Whether the world has finished initialising. |
| `render` | `RendererT*` | The renderer. |
| `physics` | `PhysicsInterface*` | The physics interface. |
| `gamedata` | `GameDataManager` | Primary game data. |
| `leveldata` | `GameDataManager` | Level-specific data. |
| `savedata` | `GameDataManager` | Save-game data. |
| `theFactory` | `RootObjectFactory*` | Object factory. |
| `factionMgr` | `FactionManager*` | Faction manager. |
| `steamEnabled` | `bool` | True if Steam platform is active. |
| `baseMods` | `lektor<ModInfo>` | List of base mods. |
| `activeMods` | `lektor<ModInfo*>` | List of currently active mods. |
| `player` | `PlayerInterface*` | Player interface. |
| `frameSpeedMult` | `float` | Current game speed multiplier. |
| `paused` | `bool` | True if game is paused. |
| `debugFlag` | `bool` | Debug mode flag. |
| `gameResetting` | `bool` | True if game is in reset state. |

**Key methods:**

| Method | Signature | Description |
|---|---|---|
| `togglePause` | `void togglePause(bool on)` | Pauses or unpauses the game. |
| `userPause` | `void userPause(bool p)` | Player-initiated pause. |
| `isPaused` | `bool isPaused() const` | Returns current pause state. |
| `setGameSpeed` | `void setGameSpeed(float speed, bool click)` | Sets the game simulation speed. |
| `getFrameSpeedMultiplier` | `float getFrameSpeedMultiplier() const` | Returns current speed multiplier. |
| `setFrameSpeedMultiplier` | `void setFrameSpeedMultiplier(float m)` | Sets speed multiplier directly. |
| `showPlayerAMessage` | `void showPlayerAMessage(const std::string&, bool queued)` | Displays a message to the player. |
| `showPlayerAMessage_withLog` | `void showPlayerAMessage_withLog(const std::string&, bool queued)` | Same, also logs the message. |
| `playNotification` | `void playNotification(const char* sound) const` | Plays a notification sound. |
| `resetGame` | `void resetGame()` | Resets the game world. |
| `dailyUpdates` | `void dailyUpdates()` | Runs daily update logic. |
| `isLoadingFromASaveGame` | `bool isLoadingFromASaveGame()` | True if currently loading a save. |
| `getCameraCenter` | `Ogre::Vector3 getCameraCenter() const` | Returns the camera's look target. |
| `getCameraPos` | `Ogre::Vector3 getCameraPos() const` | Returns the camera's world position. |
| `getWindSpeed` | `float getWindSpeed(const Ogre::Vector3& pos) const` | Returns wind speed at a position. |
| `getLightLevel` | `float getLightLevel(const Ogre::Vector3&, int floor, bool inside) const` | Returns ambient light at a position. |
| `getTimeStamp` | `double getTimeStamp()` | Returns a timestamp (game-time stamp). |
| `getLengthOfHourInRealSeconds` | `float getLengthOfHourInRealSeconds()` | Returns real-time length of one in-game hour. |
| `getObjectsWithinSphere` | `void getObjectsWithinSphere(lektor<RootObject*>&, ...)` | Spatial query for objects in a sphere. |
| `getCharactersWithinSphere` | `void getCharactersWithinSphere(lektor<RootObject*>&, ...)` | Spatial query for characters in a sphere. |
| `getObjectsWithinBox` | `void getObjectsWithinBox(lektor<RootObject*>&, ...)` | Spatial query for objects in a box. |
| `getModIndex` | `int getModIndex(const std::string& modName) const` | Returns index of a mod by name. |
| `getAllModsList` | `const lektor<ModInfo*>& getAllModsList() const` | Returns ordered list of all available mods. |
| `log` | `void log(const std::string& line)` | Logs a line to the game log. |
| `errorD` | `void errorD(const std::string& msg)` | Logs a debug error. |
| `destroy (RootObject)` | `bool destroy(RootObject*, bool justUnloaded, const char* debugInfo)` | Destroys a game object. |
| `addToUpdateListMain` | `void addToUpdateListMain(Character*)` | Adds a character to the main update list. |
| `removeFromUpdateListMain` | `void removeFromUpdateListMain(Character*)` | Removes a character from the main update list. |

#### Nested: `GameWorld::SysMessage`

Internal system message structure. Members: `msg` (`SysMessageEnum`), `on` (bool), `number` (float), `data` (void*).

#### Enum: `GameWorld::SysMessageEnum`

`SYS_NONE`, `SYS_RESIDENT_CHANGE`, `SYS_SHOW_PROSPECTING_WINDOW`, `SYS_CREATE_PHYSICAL`, `SYS_DESTROY_PHYSICAL`, `SYS_SET_CHAINED_MODE`, `SYS_UPDATE_ORDERS_PANEL`, `SYS_UPDATE_PORTRAIT`, `SYS_RESEARCH_PROGRESS`, `SYS_CLOSE_ALL_INVENTORIES`, `SYS_CLOSE_INVENTORY`, `SYS_BUILDING_PROGRESS`, `SYS_DROP_ALL_GEAR`, `SYS_CHARACTER_PARTICLES`, `SYS_RESTORE_SQUAD`, `SYS_DESTROY_PLATOON`, `SYS_BREAK_BUILDING`

#### Enum: `NxShapesType`
`NX_STATIC_SHAPES`, `NX_DYNAMIC_SHAPES`, `NX_ALL_SHAPES`

### `SimpleTimeStamper` class

| Method | Description |
|---|---|
| `getTime(double lastStamp)` | Returns elapsed time since a stamp. |
| `stampTime()` | Records and returns the current time. |

---

## Game Data

### `GameData` class

Base class for all data assets. Key accessible methods (suppressed members excluded):

| Method | Description |
|---|---|
| (data members inherited from `GameDataHeader`) | Name, string ID, category, dependencies. |

Note: `ItemData`, `ObjectInstance`, `getHandleList`, `storeHandleList`, and `objectReferences` are suppressed.

### `GameDataManager` class

| Method | Description |
|---|---|
| Accessible members per header | Container for `GameData` objects. `getBuildings` is suppressed. |

### `GameDataReference` class

Constructors, destructor, `operator=`, and `getPtr` are suppressed. Treat as opaque.

### `ModInfo` class

Fully exposed (per `ModInfo.i`). Represents metadata for a loaded mod (name, path, etc.).

### `GameSaveState` class

`states`, `getAllStates`, `pos`, and `rot` are suppressed. Other members accessible.

---

## Root Object Hierarchy

### `RootObjectBase` class

Base class. `pos` and `handle` are suppressed (opaque hand type).

### `RootObject` class

Fully exposed (per `RootObject.i`). Inherits `RootObjectBase`. Represents any object in the game world.

### `RootObjectFactory` class

`todoMutex` is suppressed. Manages creation of game objects.

---

## Character System

### `Character` class (extends `RootObject`)

**Accessible data members:**

| Member | Type | Description |
|---|---|---|
| `frameTIME` | `float` | Current frame time for this character. |
| `stealthMode` | `bool` | True if character is sneaking. |
| `_currentProneState` | `ProneState` | Current prone/KO state. |
| `sex` | `std::string` | Character's sex string. |
| `isChained` | `bool` | True if character is shackled. |
| `isCarryingSomething` | `bool` | True if carrying another character. |
| `carringObjectLeftOrRight` | `std::string` | Which side carry is on. |
| `isCarryingLeftSide` | `bool` | True if carrying on the left. |
| `inSomething` | `UseStuffState` | Whether in a bed, prison, or nothing. |
| `squadMemberID` | `int` | ID within squad. |
| `diplomacyMultiplier` | `float` | Diplomacy influence multiplier. |
| `inventory` | `Inventory*` | Character's inventory. |
| `stats` | `CharStats*` | Character's stats object. |
| `movement` | `CharMovement*` | Movement controller. |
| `myRace` | `RaceData*` | Race data for this character. |
| `isPhysicalMode` | `bool` | True if character has a physical body active. |

**Key methods:**

| Method | Description |
|---|---|
| `isDead()` | Returns true if the character is dead. |
| `isInjured(bool robot)` | True if character has injuries. |
| `isPlayerCharacter()` | True if this is a player-owned character. |
| `isEnemy(Character*, bool factorInDisguises)` | Checks enemy relationship. |
| `isAlly(Character*, bool factorInDisguises)` | Checks ally relationship. |
| `isChainedMode()` | True if in slave/chain mode. |
| `isSlave()` | Returns `SlaveStateEnum` for slave status. |
| `isBeingCarried()` | True if being carried. |
| `hasPlatoon()` | True if in a platoon. |
| `getPlatoon()` | Returns `ActivePlatoon*`. |
| `getSquadLeader()` | Returns the squad leader `Character*`. |
| `getAppearance()` | Returns `AppearanceBase*`. |
| `getRadius()` | Returns collision radius. |
| `getUpperBodyArmour()` | Returns upper body armour `Item*`. |
| `getLowerBodyArmour()` | Returns lower body armour `Item*`. |
| `getTotalCarryWeight()` | Returns total carry weight. |
| `getDiplomacyMultiplier()` | Returns diplomacy multiplier value. |
| `getRace()` | Returns `RaceData*`. |
| `getMainArmourType()` | Returns `ArmourType`. |
| `getGroundType()` | Returns `GroundType`. |
| `setGroundType(GroundType)` | Sets ground type. |
| `pickupObject(Character*)` | Makes this character pick up another. |
| `dropCarriedObject(bool ragdollHim, bool removeOnly)` | Drops a carried character. |
| `setPrisonMode(bool on, UseableStuff*)` | Puts character in/out of prison mode. |
| `setBedMode(bool on, UseableStuff*)` | Puts character in/out of bed mode. |
| `setChainedMode(bool on, const hand&)` | Toggles slave chain mode. |
| `changeSlaveOwner(const hand&)` | Reassigns slave owner. |
| `startEffect(GameData*)` | Starts a status effect on the character. |
| `stopEffect(GameData*)` | Stops a status effect. |
| `stopAllEffects()` | Removes all active effects. |
| `audioEvent(const char*, SoundRange)` | Fires an audio event. |
| `equipItem(const std::string&, Item*)` | Equips an item into a section. |
| `unequipItem(const std::string&, Item*)` | Removes an item from a section. |
| `updatePortraitGUIState()` | Forces portrait GUI refresh. |
| `canTakePlayerOrdersAtThisTime()` | True if player orders can be issued. |
| `shouldUseRangedWeapons()` | True if ranged combat is preferred. |
| `recalculateTotalEquipmentSkillBonus()` | Recalculates equipment skill bonuses. |

**Suppressed:** `ragdollMessages`, `activeEffects`, `particleEffects`, `inWhat`, `slaveOwner`, `carryingObject`, `messageSubject`, `_destinationInsideBuilding`, `msgCarryMode`, `audioData`, `audioObject`, `audioEmitter`, `medical`, `getBoneWorldPosition`, `getPredictedPosition`, `getAllAttackers`, `ragdollNavmeshPosition`, nested `RagdollMsg`, `CarryMsg`, `CharMessage`, `WhoSeesMe`.

### `CharacterHuman` class (extends `Character`)

Fully exposed per header. Represents a humanoid character.

### `CharacterAnimal` class (extends `Character`)

`itemInMouthTimeStamp` and `audioTimeStamp` are suppressed.

### `CharStats` class

`_weatherProtections`, `getTotalAttackDamageFor`, `attacks`, `blocks` are suppressed.

### `CharMovement` class

`physHit::shape/hitObject/_hitObjectUnsafePtr/normal/position`, `SpeedGroup::members/position/direction` suppressed. Copy constructor suppressed (`%nocopyctor`).

### `CombatClass` class

`effectsToAdd`, `effectsPool`, `ParticlePool::particles/ParticleData/addParticle/shiftParticles` suppressed.

### `CombatTechniqueData` class

`events` and `impactPoints` suppressed.

### `MedicalSystem` class

Entire `MedicalSystem` class suppressed (`%ignore MedicalSystem`). Only `RobotLimbs` is exposed (with `states` and `items` suppressed).

---

## Enums (Character-related)

| Enum | Values |
|---|---|
| `ProneState` | `PS_NORMAL`, `PS_STAYING_LOW`, `PS_CRIPPLED`, `PS_PLAYING_DEAD`, `PS_KO` |
| `WaterState::Enum` | `NO_WATER`, `VERY_SHALLOW_WATER`, `THIGH_DEEP_WATER`, `DEEP_WATER` |
| `RagdollPart::Enum` | `NONE`, `WHOLE`, `RIGHT_ARM`, `LEFT_ARM`, `HEAD`, `RIGHT_LEG`, `LEFT_LEG`, `CARRY_MODE`, `ARMS`, `LEGS`, `ALL` |
| `CharacterPerceptionTags_ShortTerm` | `ST_NONE`, `ST_INTRUDER`, `ST_AGGRESSOR`, `ST_TEMPORARY_ALLY`, `ST_TEMPORARY_ENEMY`, `ST_PRISONER`, `ST_HAS_BEEN_LOOTED`, `ST_CRIMINAL` |
| `CharacterPerceptionTags_LongTerm` | `LT_NONE`, `LT_MY_INTRUDER`, `LT_MY_LIFESAVER`, `LT_FREED_ME`, `LT_STOLE_FROM_ME`, `LT_MY_CAPTOR`, `LT_FRIENDLY_AQUAINTANCE`, `LT_DEFEATED_MY_SQUAD_ONCE`, `LT_SQUAD_LOST_TO_ME_ONCE`, `LT_KILLED_MY_FRIEND`, `LT_I_SCREWED_THIS_GUY`, `LT_MAX` |
| `SoundRange` | `SOUNDRANGE_SHORT`, `SOUNDRANGE_LONG`, `SOUNDRANGE_ALWAYS` |
| `SquadMemberType` | `SQUAD_1`, `SQUAD_2`, `SQUAD_LEADER`, `SQUAD_SIGNALS_PLAN`, `SQUAD_SLAVE` |
| `UseStuffState` | `IN_NOTHING`, `IN_BED`, `IN_PRISON` |

---

## Faction System

### `Faction` class

`BuildingSwaps`, `CharacteristicsData`, `ranks`, `createNewEmptyUnloadedPlatoon`, `createNewEmptyActivePlatoon`, `createPlatoonUnloaded`, `createPlatoonAuto`, `createPlatoonsAuto`, `addListMuto` suppressed. Copy constructor suppressed.

### `FactionManager` class

`addListMuto` suppressed. Copy constructor suppressed.

### `FactionLeader` class

`biomeTerritory` suppressed.

### `FactionRelations` class

Fully exposed per header.

### `FactionUniqueSquadManager` class

`squads` and `UniqueSpawnData::existingSquadsList` suppressed.

### `FactionWarMgr` class

`forces`, `hiredForces`, `activeCampaigns`, `myTowns`, `biomeTerritories`, `biomeNoGoZones`, `nextUpdateTime`, `triggerCampaign` suppressed.

### `TradeCulture` class

`forbiddenItems`, `illegalBuildings`, `happyBuildings`, `illegalItems`, `tradeGoodsMults`, `getForbiddenItemsList`, `getAllIllegalItems` suppressed.

### `FitnessSelector<T>` (template)

No instantiations exposed yet. Placeholder; specific instantiations must be declared as needed via `%template`.

---

## Items and Inventory

### `InventoryItemBase` class

`inventoryPos` suppressed.

### `Item` class (extends `InventoryItemBase`)

`stolenFrom` suppressed. Other members accessible.

### `Gear` class

Fully exposed.

### `Inventory` class

`InventorySection::SectionItem` suppressed.

---

## Buildings

### `Building` class

Many Ogre/render members suppressed: `usageNodesIds`, `usageNodes`, `badNodes`, `activeEffects`, `effects`, `AABB`, `infoWindow`, `physical`, `soundEmitter`, `rootNode`, `buildingsManager`, `myInterior`, `isFurnitureOf`, `baseMaterial`, `triggerVolume`, and all Ogre-type methods. Logical/game state members and methods remain accessible.

---

## Town / World

### `TownBase` class

`artifacts`, `addArtifactItem`, `delayedItemLoading`, `findAllBuildingsOfType`, `_NV_findAllBuildingsOfType`, `findAllBuildingsWithFunction`, `_NV_findAllBuildingsWithFunction` suppressed.

### `RaceData` class

`specialFoods`, `AllRaces`, `statMods`, `weatherImmunities`, `bloodColour` suppressed.

---

## Player / Input

### `PlayerInterface` class

`SelectionBox::volume`, `SelectionBox::widget`, `SelectionBox::contains`, `ContextMenu::orders`, `ContextMenu::menuGUI`, `ContextMenu::menuGUI2` suppressed.

### `InputHandler` class

`commands`, `map`, `editMap`, `events`, `keyboard`, `keyNameMap`, `Command::boolean` suppressed.

---

## Bounty / Crime

### `Bounty` class

`bountyAssignmentStartedTime` suppressed.

### `BountyManager` class

`bounties`, `crimeAgainst`, `accessPassExpirationTime`, `prisonSentenceBeganTime` suppressed.

### `WorldEventStateQuery` class

`statesData`, `uniqueNPCsAreNot`, `uniqueNPCsAre`, `towns`, `isAllyOf`, `isEnemyOf` suppressed.

### `WorldEventStateQueryList` class

`statesList` suppressed.

---

## Save System

### `SaveInfo` class

Constructors and `_CONSTRUCTOR` suppressed (not exported from KenshiLib.lib).

### `SaveManager` class

`scanGames` suppressed.

### `SaveFileSystem` class

`fileSystem`, `deletedFiles`, `paths`, `oldCurrentFolders`, `currentMutex`, `saveLog` suppressed. Copy constructor suppressed.

---

## Physics

### `PhysicsCollection` / `PhysicalEntity`

`PhysicalEntity::ent` and `parent` (Ogre `MovableObject*`) suppressed. `WeatherRegion` entirely suppressed.

### `PhysicsActual`

`NxUserTriggerReport` suppressed. Only `NxShapeFlag` enum exposed.

#### Enum: `NxShapeFlag`

`NX_TRIGGER_ON_ENTER`, `NX_TRIGGER_ON_LEAVE`, `NX_TRIGGER_ON_STAY`, `NX_TRIGGER_ENABLE`, `NX_SF_VISUALIZATION`, `NX_SF_DISABLE_COLLISION`, `NX_SF_FEATURE_INDICES`, `NX_SF_DISABLE_RAYCASTING`, `NX_SF_POINT_CONTACT_FORCE`, `NX_SF_FLUID_DRAIN`, `NX_SF_FLUID_DISABLE_COLLISION`, `NX_SF_FLUID_TWOWAY`, `NX_SF_DISABLE_RESPONSE`, `NX_SF_DYNAMIC_DYNAMIC_CCD`, `NX_SF_DISABLE_SCENE_QUERIES`, `NX_SF_CLOTH_DRAIN`, `NX_SF_CLOTH_DISABLE_COLLISION`, `NX_SF_CLOTH_TWOWAY`, `NX_SF_SOFTBODY_DRAIN`, `NX_SF_SOFTBODY_DISABLE_COLLISION`, `NX_SF_SOFTBODY_TWOWAY`

---

## Sensory Data

### `SeenSomeone` struct

`lastSeenTime` and `lastPosition` suppressed.

### `SenseItr`

Constructors suppressed (uses `ogre_unordered_map`).

---

## Resource Loading

### `ResourceLoader` class

`TextureLoadData`, `MeshLoadData`, `ResourceLoadRequestMesh`, `ResourceLoadRequestTexture`, mutex members, `loadModelEntity`, `loadModelEntityDetached`, `loadTextureUnitArray`, `EMPTY_CALLBACK`, `MeshLodStrategy` suppressed. Copy constructor suppressed.

---

## Logging

### `Logger` class

Fully exposed per header.

---

## Threading

### `ThreadClass` class

`runMute` and `lockedWhileRunningMute` (Boost `shared_mutex`) suppressed. Copy constructor suppressed.

---

## Options

### `OptionsHolder` class

`compositors` suppressed.

---

## Global Constants

### `GlobalConstants` class

Fully exposed per header.

---

## Instance / Handles

### `InstanceID` class

Fully exposed per header.

---

## Dialogue

The `Dialogue` class itself is entirely suppressed. Only the following enums are exposed:

### Enum: `TalkerEnum`

`T_ME`, `T_TARGET`, `T_TARGET_IF_PLAYER`, `T_INTERJECTOR1`, `T_INTERJECTOR2`, `T_INTERJECTOR3`, `T_WHOLE_SQUAD`, `T_TARGET_WITH_RACE`

### Enum: `DialogActionEnum`

`DA_NONE`, `DA_TRADE`, `DA_TALK_TO_LEADER`, `DA_JOIN_SQUAD_WITH_EDIT`, `DA_AFFECT_RELATIONS`, `DA_AFFECT_REPUTATION`, `DA_ATTACK_CHASE_FOREVER`, `DA_GO_HOME`, `DA_TAKE_MONEY`, `DA_GIVE_MONEY`, `DA_PAY_BOUNTY`, `DA_CHARACTER_EDITOR`, `DA_FORCE_SPEECH_TIMER`, `DA_DECLARE_WAR`, `DA_END_WAR`, `DA_CLEAR_AI`, `DA_FOLLOW_WHILE_TALKING`, `DA_THUG_HUNTER`, `DA_JOIN_SQUAD_FAST`, `DA_REMEMBER_CHARACTER`, `DA_FLAG_TEMP_ALLY`, `DA_FLAG_TEMP_ENEMY`, `DA_MATES_KILL_ME`, `DA_MAKE_TARGET_RUN_FASTER`, `DA_GIVE_TARGET_MY_SLAVES`, `DA_TAG_ESCAPED_SLAVE`, `DA_FREE_TARGET_SLAVE`, `DA_MERGE_WITH_SIMILAR_SQUADS`, `DA_SEPARATE_TO_MY_OWN_SQUAD`, `DA_ARREST_TARGET`, `DA_ARREST_TARGETS_CARRIED_PERSON`, `DA_ATTACK_TOWN`, `DA_ASSIGN_BOUNTY`, `DA_CRIME_ALARM`, `DA_RUN_AWAY`, `DA_INCREASE_FACTION_RANK`, `DA_LOCK_THIS_DIALOG`, `DA_ASSAULT_PHASE`, `DA_RETREAT_PHASE`, `DA_VICTORY_PHASE`, `DA_ENSLAVE_TARGETS_CARRIED_PERSON`, `CHOOSE_SLAVES_SELLING`, `CHOOSE_SLAVES_BUYING`, `CHOOSE_PRISONER_BAIL`, `CHOOSE_CONSCRIPTION`, `CHOOSE_RECRUITING`, `CHOOSE_HIRING_CONTRACT`, `SURRENDER_NON_HUMANS`, `CHOOSE_ANIMALS_BUYING`

---

## Tasks

`Tasks.h` is currently an empty header placeholder. Nothing is exposed from this module.

---

## GUI

### `TitleScreen` class

Exposed per header (via `Titlescreen.i`). Represents the game's title screen object.

---

## Game Launcher (Partial)

The `GameLauncher` class and `CExCheckListBox` are fully suppressed (MFC/ATL types). Only the following enum is exposed:

### Enum: `GameLauncher_RendererType`

`RENDERER_DX9 = 0`, `RENDERER_DX11 = 1`

---

## Damages

### `Damages` class

Constructors and destructors suppressed (private Ogre allocator). `CharStats::getTotalAttackDamageFor` also suppressed. Fields accessible where Ogre allocator is not required.

---

## Utility Types

### `TagsClass`

Exposed per header (`TagsClass.i`).

### Enums from `Enums.h`

All enums in `Enums.h` are fully exposed (no suppressions in `Enums.i`). This includes engine-level enumerations for item types, weapon categories, armour types, ground types, weather, and more.

---

## Notes on Suppressed Functionality

The following categories of things are systematically suppressed across all bindings:

- **Boost types** (`shared_mutex`, `function`, etc.) — not wrappable via SWIG.
- **Ogre internal types** (`MovableObject*`, `PlaneBoundedVolume`, `ogre_unordered_map/set`, `lektor` where it contains complex types) — opaque to SWIG.
- **PhysX internal types** (`NxScene`, `NxActor`, `NxShape`, etc.).
- **MyGUI types** — suppressed throughout.
- **Raw array members** where SWIG cannot determine size.
- **Members not exported from `KenshiLib.lib`** (static members flagged as such).
- **`hand` type members** in most classes (the game's internal handle/pointer wrapper).
