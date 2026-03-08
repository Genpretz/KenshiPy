%{
#include "kenshi\FactionWarMgr.h"
%}

%include "std_string.i"

%ignore FactionWarMgr::forces;
%ignore FactionWarMgr::hiredForces;
%ignore FactionWarMgr::activeCampaigns;
%ignore FactionWarMgr::myTowns;
%ignore FactionWarMgr::biomeTerritories;
%ignore FactionWarMgr::biomeNoGoZones;
%ignore FactionWarMgr::nextUpdateTime;
%ignore FactionWarMgr::triggerCampaign;

%include "kenshi\FactionWarMgr.h"
