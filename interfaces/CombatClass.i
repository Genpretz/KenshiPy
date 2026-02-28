%{
#include "CharMovement_guarded.h"
#include "CombatClass.h"
%}

%include "std_string.i"
%include "stdint.i"

%import "CharMovement.h"

// ParticlePool uses internal Ogre particle types
%ignore ParticlePool::particles;
%ignore ParticlePool::ParticleData;
%ignore ParticlePool::addParticle;
%ignore ParticlePool::shiftParticles;

// Static members not exported from KenshiLib.lib
%ignore CombatClass::effectsToAdd;
%ignore CombatClass::effectsPool;

%include "CombatClass.h"
