%{
#include "kenshi/ResourceLoader.h"
%}

%include "std_string.i"

// Suppress Ogre resource types, Boost function, and internal structs
%ignore ResourceLoader::TextureLoadData;
%ignore ResourceLoader::MeshLoadData;
%ignore ResourceLoader::ResourceLoadRequestMesh;
%ignore ResourceLoader::ResourceLoadRequestTexture;
%ignore ResourceLoader::loadingMeshQueueMutex;
%ignore ResourceLoader::texturesLoadingMutex;
%ignore ResourceLoader::texturesLoadedMutex;
%nocopyctor ResourceLoader;

// Methods using boost::function or complex Ogre types - not exportable
%ignore ResourceLoader::loadModelEntity;
%ignore ResourceLoader::loadModelEntityDetached;
%ignore ResourceLoader::loadTextureUnitArray;
%ignore ResourceLoader::EMPTY_CALLBACK;
%ignore ResourceLoader::MeshLodStrategy;

%include "kenshi/ResourceLoader.h"
