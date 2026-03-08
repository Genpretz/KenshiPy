%{
#include "kenshi\Building.h"
%}

%include "std_string.i"
%include "stdint.i"

// Suppress members using Ogre/internal container types that SWIG cannot wrap
%ignore Building::usageNodesIds;
%ignore Building::usageNodes;
%ignore Building::badNodes;
%ignore Building::activeEffects;
%ignore Building::effects;
%ignore Building::AABB;
%ignore Building::infoWindow;
%ignore Building::physical;
%ignore Building::soundEmitter;
%ignore Building::rootNode;
%ignore Building::buildingsManager;
%ignore Building::myInterior;
%ignore Building::isFurnitureOf;
%ignore Building::baseMaterial;
%ignore Building::triggerVolume;

// Suppress methods that return/take Ogre types not handled
%ignore Building::getAABB;
%ignore Building::setAABB;
%ignore Building::getPartMaterial;
%ignore Building::getBuildingPartMaterial;
%ignore Building::setBuildingPartMaterial;
%ignore Building::getLights;
%ignore Building::getPositionMarker;
%ignore Building::getDirectionMarker;
%ignore Building::getDirectionMarkerQuat;
%ignore Building::calculateLeftPost;
%ignore Building::loadInstances;
%ignore Building::loadParts;
%ignore Building::loadPartEntity;
%ignore Building::loadEntity;
%ignore Building::loadEntityCallback;
%ignore Building::selectParts;
%ignore Building::findPhysicalEntity;
%ignore Building::findPhysicalEntityInCollection;
%ignore Building::buildingContainsEntity;
%ignore Building::getEntityMaterialName;
%ignore Building::setPartVisible;
%ignore Building::restoreMaterialToPhysical;
%ignore Building::getRootNode;
%ignore Building::positionMarker;
%ignore Building::refreshGUIChangesFlag;

%include "kenshi\Building.h"
