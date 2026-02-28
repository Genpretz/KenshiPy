%{
#include "MedicalSystem.h"
%}

%include "std_string.i"

%ignore MedicalSystem;

// RobotLimbs::states and items are raw arrays; suppress to avoid SWIG confusion
%ignore RobotLimbs::states;
%ignore RobotLimbs::items;

%include "MedicalSystem.h"
