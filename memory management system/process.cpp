#include <iostream>
#include "process.h"
#include "freeMemorySpace.h"

//create a process with certain size
Process::Process(list<Process*> &processList, list<FreeMemorySpace*> &freeMemorySpaceList, const int &size)
{
    base=-1;
    id=1;
    limit=size;

    bool found=true;
    //search for the smallest available ID
    while (found)
    {
        found=false;
        for (list<Process*>::iterator i=processList.begin();i!=processList.end();++i)
        {
            if (id==(**i).GetID())
            {
                ++id;
                found=true;
                break;
            }
        }
    }

    //check the free memory space list to allocate for this process
    for (list<FreeMemorySpace*>::iterator i=freeMemorySpaceList.begin();i!=freeMemorySpaceList.end();++i)
    {
        if ((**i).GetSize()<limit)
        {
            continue;
        }
        else
        {
            base=(**i).GetBase()+(**i).GetSize()-limit;
            (**i).Allocate(*this);
            if ((**i).GetSize()==0)
            {
                FreeMemorySpace* toDelete=*i;
                freeMemorySpaceList.erase(i);
                delete toDelete;
                toDelete=0;
            }
            break;
        }
    }

    //if no enough space
    if (base==-1)
    {
        cout<<"\nNo enough memory space!\n";
    }
    else
    {
        cout<<"\nProcess(ID: "<<id<<") has been created\n";
    }
}

//terminate a process
void Process::Exit(list<FreeMemorySpace*> &freeMemorySpaceList)
{
    //create a new free memory space
    FreeMemorySpace* newFreeMemorySpace=new FreeMemorySpace(base, limit);
    //merge it with adjacent free memory spaces
    (*newFreeMemorySpace).Merge(freeMemorySpaceList);
    //add it to the free memory space list
    freeMemorySpaceList.push_back(newFreeMemorySpace);
}

ostream& operator<<(ostream& out, const Process &process)
{
    out<<"Process ID: "<<process.GetID()<<"\tBase Address: "<<process.GetBase()<<"\tLimit: "<<process.GetLimit();
    return out;
}
