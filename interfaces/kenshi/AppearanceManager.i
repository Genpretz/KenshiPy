%{
#include "kenshi\AppearanceManager.h"
%}

%ignore AppearanceManager::APPEARANCE_IN_EDITOR_KEY;
%ignore AppearanceManager::APPEARANCE_FROM_FILE_KEY;
%ignore AppearanceManager::setMaterialTextures;
%ignore AppearanceManager::getEditorData;

%include "kenshi\AppearanceManager.h"