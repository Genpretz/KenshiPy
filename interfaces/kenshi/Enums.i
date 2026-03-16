%{
#include "kenshi/Enums.h"
%}

// EffectType is a namespace enum - prefix values to avoid collision
// with itemType (CAMERA, NONE clash)
%rename(EFFECT_NONE)          EffectType::NONE;
%rename(EFFECT_CAMERA)        EffectType::CAMERA;
%rename(EFFECT_POINT)         EffectType::POINT;
%rename(EFFECT_WANDERING)     EffectType::WANDERING;
%rename(EFFECT_GLOBAL)        EffectType::GLOBAL;
%rename(EFFECT_CAMERA_RAIN)   EffectType::CAMERA_RAIN;
%rename(EFFECT_CAMERA_ACID_RAIN) EffectType::CAMERA_ACID_RAIN;
%rename(EFFECT_POINT_LIGHTING) EffectType::POINT_LIGHTING;
%rename(EFFECT_WANDERING_STORM) EffectType::WANDERING_STORM;
%rename(EFFECT_WANDERING_GAS) EffectType::WANDERING_GAS;
%rename(EFFECT_GLOBAL_POINT)  EffectType::GLOBAL_POINT;

%include "kenshi/Enums.h"