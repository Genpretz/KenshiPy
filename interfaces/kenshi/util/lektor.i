%{
#include "kenshi/util/lektor.h"
%}

// SWIG sees only this simplified declaration - no Ogre base class,
// no function pointer members, no allocator complexity.
// The real lektor<T> is ABI-compatible because the public members
// (count, maxSize, stuff) are identical.

#include "kenshi/util/lektor.h"

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

// Explicit instantiations - add here as new lektor<T> return types are exposed.
// lektor<ModInfo*> is used by GameWorld::activeMods
%template(lektor_ModInfoPtr)    lektor<ModInfo*>;
// lektor<Character*> is returned by PlayerInterface::getAllPlayerCharacters() const
%template(lektor_CharacterPtr)  lektor<Character*>;
// lektor<RootObject*> is used by the out-param overload and RootObjectContainer
%template(lektor_RootObjectPtr) lektor<RootObject*>;