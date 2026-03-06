%{
#include "kenshi/util/lektor.h"
%}

// SWIG sees only this simplified declaration - no Ogre base class,
// no function pointer members, no allocator complexity.
// The real lektor<T> is ABI-compatible because the public members
// (count, maxSize, stuff) are identical.
template <typename T>
class lektor
{
public:
    unsigned int size() const;
    unsigned int capacity() const;
    bool valid() const;
    void clear();

    %extend {
        T __getitem__(unsigned int index) {
            return (*$self)[index];
        }
    }
};
