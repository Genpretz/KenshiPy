%{
#include "kenshi/MedicalSystem.h"
%}

// Include enums so SWIG knows LimbState and Limb
%include "kenshi/Enums.h"

// Ignore nested classes
%ignore MedicalSystem::HealthPartStatus;

%ignore LimbsInventoryLayout::_NV_setupSections; // unresolved external symbol

// Prefix AttackDirection enums to avoid collisions
%rename(ATTACK_DIR_FRONT)  AttackDirection::FRONT;
%rename(ATTACK_DIR_BACK)   AttackDirection::BACK;
%rename(ATTACK_DIR_LEFT)   AttackDirection::LEFT;
%rename(ATTACK_DIR_RIGHT)  AttackDirection::RIGHT;
%rename(ATTACK_DIR_TOP)    AttackDirection::TOP;
%rename(ATTACK_DIR_BOTTOM) AttackDirection::BOTTOM;

// Ignore raw arrays
%ignore RobotLimbs::states;
%ignore RobotLimbs::items;

// Extend the original class to provide Python-friendly access
%extend RobotLimbs {
    LimbState getState(RobotLimbs::Limb limb) const {
        if (limb < RobotLimbs::LEFT_ARM || limb > RobotLimbs::RIGHT_LEG) return LIMB_ORIGINAL;
        return $self->states[limb];
    }

    void setState(RobotLimbs::Limb limb, LimbState state) {
        if (limb < RobotLimbs::LEFT_ARM || limb > RobotLimbs::RIGHT_LEG) return;
        $self->states[limb] = state;
    }

    Item* getLimb(RobotLimbs::Limb limb) const {
        if (limb < RobotLimbs::LEFT_ARM || limb > RobotLimbs::RIGHT_LEG) return nullptr;
        return $self->items[limb];
    }

    void setLimb(RobotLimbs::Limb limb, LimbState state, Item* item) {
        if (limb < RobotLimbs::LEFT_ARM || limb > RobotLimbs::RIGHT_LEG) return;
        $self->states[limb] = state;
        $self->items[limb] = item;
    }
}

// Wrap the rest of MedicalSystem normally
%include "kenshi/MedicalSystem.h"


