%{
#include "Globals.h"
%}

// These are __declspec(dllimport) globals; expose as pointer accessors.
// SWIG cannot wrap dllimport variables directly, so declare them as opaque
// pointers and provide getter wrappers inline.

%inline %{
    GameWorld*      getGameWorld()       { return ou; }
    GlobalConstants* getGlobalConstants() { return con; }
    InputHandler*   getInputHandler()    { return key; }
    OptionsHolder*  getOptionsHolder()   { return options; }
%}

// Free functions
void  showErrorMessage();
float modMedicalSkill(float skill, Item* equipment, float frameTIME);
