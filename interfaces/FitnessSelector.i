%{
#include "FitnessSelector.h"
%}

// FitnessSelector is a template class; SWIG cannot instantiate it generically.
// Explicit instantiations must be declared per use-site.
// This file is intentionally minimal -- specific instantiations go in their
// respective .i files if needed.

// Example (uncomment and adapt as required):
// %template(FitnessSelectorGameData) FitnessSelector<GameData*>;
