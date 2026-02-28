%{
#include "WorldEventStateQuery.h"
%}

%include "std_string.i"

%ignore WorldEventStateQuery::statesData;
%ignore WorldEventStateQuery::uniqueNPCsAre;
%ignore WorldEventStateQuery::uniqueNPCsAreNot;
%ignore WorldEventStateQuery::towns;
%ignore WorldEventStateQuery::isAllyOf;
%ignore WorldEventStateQuery::isEnemyOf;
%ignore WorldEventStateQueryList::statesList;

%include "WorldEventStateQuery.h"
