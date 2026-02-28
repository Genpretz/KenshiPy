%{
#include "InputHandler.h"
%}

%include "std_string.i"

%ignore InputHandler::commands;
%ignore InputHandler::map;
%ignore InputHandler::editMap;
%ignore InputHandler::events;
%ignore InputHandler::keyboard;
%ignore InputHandler::keyNameMap;
%ignore InputHandler::Command::boolean;

%include "InputHandler.h"
