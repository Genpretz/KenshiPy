%{
#include "CharMovement_guarded.h"
#include "kenshi\Character.h"
%}

%include "std_string.i"
%include "stdint.i"

// CharMovement.h is already fully processed via CharMovement.i
%import "kenshi\CharMovement.h"

// RagdollPart is a namespace enum - prefix all values to avoid collision
// with itemType (HEAD clash) and RobotLimbs::Limb (RIGHT_ARM etc clash)
%rename(RAGDOLL_NONE)      RagdollPart::NONE;
%rename(RAGDOLL_WHOLE)     RagdollPart::WHOLE;
%rename(RAGDOLL_RIGHT_ARM) RagdollPart::RIGHT_ARM;
%rename(RAGDOLL_LEFT_ARM)  RagdollPart::LEFT_ARM;
%rename(RAGDOLL_HEAD)      RagdollPart::HEAD;
%rename(RAGDOLL_RIGHT_LEG) RagdollPart::RIGHT_LEG;
%rename(RAGDOLL_LEFT_LEG)  RagdollPart::LEFT_LEG;
%rename(RAGDOLL_CARRY_MODE) RagdollPart::CARRY_MODE;
%rename(RAGDOLL_ARMS)      RagdollPart::ARMS;
%rename(RAGDOLL_LEGS)      RagdollPart::LEGS;
%rename(RAGDOLL_ALL)       RagdollPart::ALL;

// Suppress members using internal container/utility types
%ignore Character::ragdollMessages;
%ignore Character::activeEffects;
%ignore Character::particleEffects;
%ignore Character::inWhat;
%ignore Character::messageSubject;
%ignore Character::msgCarryMode;
%ignore Character::audioData;
%ignore Character::audioObject;
%ignore Character::audioEmitter;

// Suppress methods using Ogre/internal types
%ignore Character::ragdollNavmeshPosition;

// Suppress nested structs with complex types
%ignore Character::RagdollMsg;
%ignore Character::CarryMsg;
%ignore Character::CharMessage;
%ignore Character::WhoSeesMe;
%ignore Character::whoSeesMeSneaking;

%include "kenshi\Character.h"