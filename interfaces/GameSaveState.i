%{
#include "GameSaveState.h"
%}

%include "std_string.i"

%ignore GameSaveState::states;
%ignore GameSaveState::getAllStates;
%ignore GameSaveState::pos;
%ignore GameSaveState::rot;

%include "GameSaveState.h"
