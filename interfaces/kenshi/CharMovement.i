%{
#include "CharMovement_guarded.h"
%}

%include "std_string.i"
%include "stdint.i"

// Suppress PhysX and internal types SWIG cannot wrap
%ignore physHit::shape;
%ignore physHit::hitObject;
%ignore physHit::_hitObjectUnsafePtr;
%ignore physHit::normal;
%ignore physHit::position;
%ignore SpeedGroup::members;
%ignore SpeedGroup::position;
%ignore SpeedGroup::direction;
%nocopyctor CharMovement;

%include "CharMovement_guarded.h"
