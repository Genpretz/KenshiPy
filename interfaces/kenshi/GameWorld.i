%{
#include "kenshi/GameWorld.h"
%}

/* =========================
      WARNING SUPPRESSION
   ========================= */

/* 401 = Nothing known about base class */
%warnfilter(401) GameWorld;

/* 314 = Python keyword rename (e.g., 'from') */
%warnfilter(314) GameWorld::SysMessage::from;

/* 325 = Nested class not supported */
%warnfilter(325) GameWorld::SysMessage;

/* =========================
      IGNORE NESTED CLASS
   ========================= */

%ignore GameWorld::SysMessage;

%ignore GameWorld::SysMessage::from;

/* All functions depending on SysMessage */
%ignore GameWorld::sysMessage;
%ignore GameWorld::sysMessageUrgent;
%ignore GameWorld::sysMessage_noDuplicates;

/* =========================
   IGNORE OGRE-DEPENDENT API
   ========================= */

%ignore GameWorld::destroy(Ogre::MovableObject*);
%ignore GameWorld::getObjectsWithinSphere;
%ignore GameWorld::getCharactersWithinSphere;
%ignore GameWorld::getObjectsWithinBox;
%ignore GameWorld::buildingIntersectionTestCapsule;
%ignore GameWorld::getLightLevel;
%ignore GameWorld::findValidSpawnPos;
%ignore GameWorld::getCameraCenter;
%ignore GameWorld::getCameraPos;
%ignore GameWorld::fixNaNPosition;
%ignore GameWorld::getWindSpeed;

/* =========================
    IGNORE CUSTOM CONTAINERS
   ========================= */

//%ignore GameWorld::getModsListFromConfig;
//%ignore GameWorld::getAllModsList;
//%ignore GameWorld::getCharacterUpdateList;

/* =========================
   IGNORE UNKNOWN VALUE TYPES
   ========================= */

//%ignore GameWorld::getTimeFromStamp(const TimeOfDay&);
//%ignore GameWorld::getTimeStamp_inGameHours();

/* =========================
 OPTIONAL: OPAQUE POINTER TYPES
   ========================= */

/* Prevent SWIG from trying to fully wrap these */
//%opaque RendererT;
//%opaque GameDataManager;
//%opaque RootObject;
//%opaque Character;
//%opaque Platoon;
//%opaque ZoneMap;
//%opaque ThreadWannabe;

/* =========================
  OPTIONAL RENAMES (API CLEANUP)
   ========================= */

/* Example: cleaner Python naming if desired
%rename(start_game) GameWorld::start;
%rename(reset) GameWorld::resetGame;
%rename(is_paused) GameWorld::isPaused;
%rename(set_speed) GameWorld::setGameSpeed;
%rename(log_debug) GameWorld::logDebug; */

/* =========================
        INCLUDE HEADER
   ========================= */

%include "std_string.i"
%include "kenshi/GameWorld.h"