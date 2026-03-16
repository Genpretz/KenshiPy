%{
#include "kenshi\Appearance.h"
%}

%ignore AppearanceBase::woundsListMutex;
%ignore AppearanceBase::_NV_setupCharacterEntityTexture;

%include "kenshi\Appearance.h"