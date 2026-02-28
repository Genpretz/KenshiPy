%{
#include "Item.h"
%}

%include "std_string.i"
%include "stdint.i"

%ignore InventoryItemBase::inventoryPos;
%ignore Item::stolenFrom;

%include "Item.h"
