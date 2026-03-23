%{
#include "kenshi/Globals.h"
%}

// Globals.h only forward-declares these types.
// Import the full definitions so SWIG returns proper wrapped objects
// instead of opaque SwigPyObject pointers.
%import "kenshi/GameWorld.h"
%import "kenshi/GlobalConstants.h"
%import "kenshi/InputHandler.h"
%import "kenshi/OptionsHolder.h"

// Free functions
void  showErrorMessage();
float modMedicalSkill(float skill, Item* equipment, float frameTIME);

// These are __declspec(dllimport) globals; expose as pointer accessors.
// SWIG cannot wrap dllimport variables directly, so declare them as opaque
// pointers and provide getter wrappers inline.
%inline %{
    GameWorld*       getGameWorld()       { return ou; }
    GlobalConstants* getGlobalConstants() { return con; }
    InputHandler*    getInputHandler()    { return key; }
    OptionsHolder*   getOptionsHolder()   { return options; }
%}

// Expose these at module level with the original names
%pythoncode %{
# Module-level aliases so you can use KenshiPy.ou directly
ou = getGameWorld
con = getGlobalConstants
key = getInputHandler
options = getOptionsHolder
%}
