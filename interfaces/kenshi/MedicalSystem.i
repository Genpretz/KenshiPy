%{
#include "kenshi/MedicalSystem.h"
%}

%include "std_string.i"

%ignore MedicalSystem;

// RobotLimbs::states and items are raw arrays; suppress to avoid SWIG confusion
%ignore RobotLimbs::states;
%ignore RobotLimbs::items;

// AttackDirection is a namespace enum - prefix all values to avoid collision
// with CutOrigination (FRONT clash) and other flat enums
%rename(ATTACK_DIR_FRONT)  AttackDirection::FRONT;
%rename(ATTACK_DIR_BACK)   AttackDirection::BACK;
%rename(ATTACK_DIR_LEFT)   AttackDirection::LEFT;
%rename(ATTACK_DIR_RIGHT)  AttackDirection::RIGHT;
%rename(ATTACK_DIR_TOP)    AttackDirection::TOP;
%rename(ATTACK_DIR_BOTTOM) AttackDirection::BOTTOM;

%include "kenshi/MedicalSystem.h"