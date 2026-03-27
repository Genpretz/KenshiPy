%{
#include "kenshi\AppearanceManager.h"
%}

// ignore nested classes (swig is not able to wrap these)
%ignore AppearanceManager::AppearanceData;
%ignore AppearanceManager::DataCategory;
%ignore AppearanceManager::DataRange;
%ignore AppearanceManager::DataRangePose;
%ignore AppearanceManager::DataRangeVector;
%ignore AppearanceManager::Gender;

%ignore AppearanceManager::APPEARANCE_IN_EDITOR_KEY;
%ignore AppearanceManager::APPEARANCE_FROM_FILE_KEY;
%ignore AppearanceManager::setMaterialTextures;
%ignore AppearanceManager::getEditorData;

%include "kenshi\AppearanceManager.h"