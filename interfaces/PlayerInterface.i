%{
#include "PlayerInterface.h"
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

%include "PlayerInterface.h"
