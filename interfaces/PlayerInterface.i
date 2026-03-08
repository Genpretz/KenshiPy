%{
#include "kenshi/PlayerInterface.h"
%}

%include "std_string.i"

// SelectionBox uses Ogre PlaneBoundedVolume; suppress it
%ignore SelectionBox::volume;
%ignore SelectionBox::widget;
%ignore SelectionBox::contains;

// ContextMenu orders list and MyGUI types
%ignore ContextMenu::orders;
%ignore ContextMenu::menuGUI;
%ignore ContextMenu::menuGUI2;

// getAllPlayerCharacters has two overloads:
//   void getAllPlayerCharacters(lektor<RootObject*>& list)   -- out-param, fills RootObject list
//   const lektor<Character*>& getAllPlayerCharacters() const -- returns wrapped Character list
// Python has no overloading, so rename the out-param version to a distinct name.
%rename(getAllPlayerCharacters_rootObjects) PlayerInterface::getAllPlayerCharacters(lektor<RootObject*>&);

%include "kenshi/PlayerInterface.h"