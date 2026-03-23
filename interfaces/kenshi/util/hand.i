%{
#include "kenshi/util/hand.h"
%}

%include "std_string.i"
%include "stdint.i"

// NULL_HAND is a dllimport static member; SWIG cannot link against it directly.
%ignore hand::NULL_HAND;




%include "kenshi/util/hand.h"
