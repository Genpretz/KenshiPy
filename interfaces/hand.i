%{
#include "hand.h"
%}

%include "std_string.i"
%include "stdint.i"

// NULL_HAND is a dllimport static member; SWIG cannot link against it directly.
%ignore hand::NULL_HAND;

// Suppress the _CONSTRUCTOR/_DESTRUCTOR RVA aliases -- these are internal
// calling-convention duplicates of the real constructors and are not needed
// in Python.
%ignore hand::_CONSTRUCTOR;
%ignore hand::_DESTRUCTOR;

// operator bool() -- rename to __bool__ so Python truthiness works correctly.
%rename(__bool__) hand::operator bool() const;

// operator= overloads -- suppress the RootObjectBase* and int& forms since
// SWIG cannot safely resolve the correct overload from Python. The copy
// assignment from hand& is retained.
%ignore hand::operator=(RootObjectBase* a);
%ignore hand::operator=(const int& a);

// operator== overloads -- suppress the bool overload (takes a bool, not a
// hand or pointer) to avoid ambiguous Python dispatch. The hand& and
// RootObjectBase* overloads are kept.
%ignore hand::operator==(bool a) const;
%ignore hand::_NV_operator_equal(bool a) const;

// Suppress _NV_ non-virtual dispatch aliases -- already covered by the
// virtual versions.
%ignore hand::_NV_operator_equal(const hand& a) const;
%ignore hand::_NV_operator_notequal(const hand& a) const;

%include "hand.h"
