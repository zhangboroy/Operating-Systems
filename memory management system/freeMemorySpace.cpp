#include "freeMemorySpace.h"

FreeMemorySpace::FreeMemorySpace(const int &base1, const int &size1)
{
    base=base1;
    size=size1;
}

//merge it with adjacent free memory spaces
void FreeMemorySpace::Merge(list<FreeMemorySpace*> &freeMemorySpaceList)
{
    //search the freeMemorySpaceList for top free space or bottom free space
    for (list<FreeMemorySpace*>::iterator i=freeMemorySpaceList.begin();i!=freeMemorySpaceList.end();)
    {
        //top free space
        if (base==(**i).GetBase()+(**i).GetSize())
        {
            //merge the size
            size+=(**i).GetSize();
            //replace the position of the top space
            base=(**i).GetBase();
            //remove the top space from the list and destroy it
            FreeMemorySpace* toBeDelete=*i;
            freeMemorySpaceList.erase(i++);
            delete toBeDelete;
            toBeDelete=0;
        }
        //bottom free space
        else if (base+size==(**i).GetBase())
        {
            //merge the size
            size+=(**i).GetSize();
            //remove the bottom space from the list and destroy it
            FreeMemorySpace* toBeDelete=*i;
            freeMemorySpaceList.erase(i++);
            delete toBeDelete;
            toBeDelete=0;
        }
        //not adjacent free spaces
        else
        {
            ++i;
        }
    }
}

ostream& operator<<(ostream& out, const FreeMemorySpace &freeMemorySpace)
{
    out<<"Free Memory Space Base Address: "<<freeMemorySpace.GetBase()<<"\tSize: "<<freeMemorySpace.GetSize();
    return out;
}
