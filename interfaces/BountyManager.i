%{
#include "kenshi\BountyManager.h"
%}

%include "std_string.i"
%include "stdint.i"

// Opaque forward declarations for types we don't wrap here
%ignore BountyManager::bounties;
%ignore BountyManager::crimeAgainst;
%ignore BountyManager::accessPassExpirationTime;
%ignore BountyManager::prisonSentenceBeganTime;

%include "kenshi\BountyManager.h"
