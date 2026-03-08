%{
#include "kenshi/TradeCulture.h"
%}

%include "std_string.i"

%ignore TradeCulture::forbiddenItems;
%ignore TradeCulture::illegalBuildings;
%ignore TradeCulture::happyBuildings;
%ignore TradeCulture::illegalItems;
%ignore TradeCulture::tradeGoodsMults;
%ignore TradeCulture::getForbiddenItemsList;
%ignore TradeCulture::getAllIllegalItems;

%include "kenshi/TradeCulture.h"
