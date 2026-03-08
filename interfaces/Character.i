%{
#include "CharMovement_guarded.h"
#include "kenshi\Character.h"
%}

%include "std_string.i"
%include "stdint.i"

// CharMovement.h is already fully processed via CharMovement.i
%import "kenshi\CharMovement.h"

// Suppress members using internal container/utility types
%ignore Character::ragdollMessages;
%ignore Character::activeEffects;
%ignore Character::particleEffects;
%ignore Character::inWhat;
%ignore Character::slaveOwner;
%ignore Character::carryingObject;
%ignore Character::messageSubject;
%ignore Character::_destinationInsideBuilding;
%ignore Character::msgCarryMode;
%ignore Character::audioData;
%ignore Character::audioObject;
%ignore Character::audioEmitter;
%ignore Character::medical;

// Suppress methods using Ogre/internal types
%ignore Character::getBoneWorldPosition;
%ignore Character::getPredictedPosition;
%ignore Character::getAllAttackers;
%ignore Character::ragdollNavmeshPosition;

// Suppress nested structs with complex types
%ignore Character::RagdollMsg;
%ignore Character::CarryMsg;
%ignore Character::CharMessage;
%ignore Character::WhoSeesMe;
%ignore Character::whoSeesMeSneaking;

%include "kenshi\Character.h"
