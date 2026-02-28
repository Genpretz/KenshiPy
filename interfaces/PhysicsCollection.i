%{
#include "PhysicsCollection.h"
%}

%include "std_string.i"

// Suppress Ogre MovableObject pointer members
%ignore PhysicalEntity::ent;
%ignore PhysicalEntity::parent;

// WeatherRegion uses internal listener lists
%ignore WeatherRegion;

%include "PhysicsCollection.h"
