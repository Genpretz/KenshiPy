#pragma once

template<typename T>
class TagsClass
{
public:
    // VTable         : (none)
    unsigned int flags; // 0x8 Member
    // no_addr void TagsClass<enum SenseType>(const class TagsClass<enum SenseType> & _a1);// public missing arg names
    TagsClass<T>();// public RVA = 0x85B840
    TagsClass<T>* _CONSTRUCTOR();// public RVA = 0x85B840
    //virtual void setTag(T f);// public RVA = 0x85B860 vtable offset = 0x0
    void _NV_setTag(T f);// public RVA = 0x85B860 vtable offset = 0x0
    void clearTag(T f);// public RVA = 0x68A260
    void clearAll();// public RVA = 0x85B880
    //virtual bool getTag(T f) const;// public RVA = 0x85B890 vtable offset = 0x8
    bool _NV_getTag(T f) const;// public RVA = 0x85B890 vtable offset = 0x8
    bool has(T t) const;// public RVA = 0x7BF6C0
    // no_addr bool hasAllOfTheseBitsSet(unsigned int _a1);// public missing arg names
    bool hasNONEOfTheseBitsSet(unsigned int flagsToTest) const;// public RVA = 0x85B8B0
    bool hasANYOfTheseBitsSet(unsigned int flagsToTest) const;// public RVA = 0x85B8C0
    // no_addr unsigned int getRawFlag();// public
    // no_addr void setRawFlag(unsigned int _a1);// public missing arg names
    // no_addr class TagsClass<enum SenseType> & operator=(const class TagsClass<enum SenseType> & _a1);// public missing arg names
};