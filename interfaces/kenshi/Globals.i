%{
#include "kenshi/Globals.h"
%}

// Tell SWIG to ignore these dllimport variables - they can't be wrapped directly
%ignore ou;
%ignore con;
%ignore key;
%ignore options;

// Import the full definitions for the types
%import "kenshi/GameWorld.h"
%import "kenshi/GlobalConstants.h"
%import "kenshi/InputHandler.h"
%import "kenshi/OptionsHolder.h"

// Free functions
void  showErrorMessage();
float modMedicalSkill(float skill, Item* equipment, float frameTIME);

// Provide accessor functions instead
%inline %{
    GameWorld*       getGameWorld()       { return ou; }
    GlobalConstants* getGlobalConstants() { return con; }
    InputHandler*    getInputHandler()    { return key; }
    OptionsHolder*   getOptionsHolder()   { return options; }
%}

// Expose at module level with original names
%pythoncode %{
ou = getGameWorld
con = getGlobalConstants
key = getInputHandler
options = getOptionsHolder
%}