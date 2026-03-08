%{
#include "kenshi\Damages.h"
%}

// Damages privately inherits Ogre::GeneralAllocatedObject which makes
// operator new/delete inaccessible. Ignore all constructors and destructor
// so SWIG does not attempt to call new/delete on this class.
%ignore Damages::Damages;
%ignore Damages::~Damages;
%ignore Damages::_CONSTRUCTOR;
%ignore Damages::_DESTRUCTOR;
%ignore CharStats::getTotalAttackDamageFor;

%include "kenshi\Damages.h"
