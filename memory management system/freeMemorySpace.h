#ifndef FREEMEMORYSPACE_H_INCLUDED
#define FREEMEMORYSPACE_H_INCLUDED

#include "process.h"

class FreeMemorySpace
{
    int base;
    int size;
public:
    FreeMemorySpace(const int &base1, const int &size1);
    int GetBase() const
    {
        return base;
    }
    int GetSize() const
    {
        return size;
    }
    //reset the base to 0
    void ResetBase()
    {
        base=0;
    }
    //allocate a process in this free space
    void Allocate(const Process &process)
    {
        size-=process.GetLimit();
    }
    //grow the size
    void Grow(const int &sizeGrown)
    {
        size+=sizeGrown;
    }
    //merge it with adjacent free memory spaces
    void Merge(list<FreeMemorySpace*> &freeMemorySpaceList);
};

ostream& operator<<(ostream& out, const FreeMemorySpace &freeMemorySpace);

#endif // FREEMEMORYSPACE_H_INCLUDED
