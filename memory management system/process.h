#ifndef PROCESS_H_INCLUDED
#define PROCESS_H_INCLUDED

#include <list>
#include <iostream>

using namespace std;

//the total memory size
static int memorySize=1024;
class FreeMemorySpace;

class Process
{
    int id;
    int base;
    int limit;
public:
    //create a process with certain size
    Process(list<Process*> &processList, list<FreeMemorySpace*> &freeMemorySpaceList, const int &size);
    int GetID() const
    {
        return id;
    }
    int GetBase() const
    {
        return base;
    }
    int GetLimit() const
    {
        return limit;
    }
    void SetBase(const int &newBase)
    {
        base=newBase;
    }
    //terminate a process
    void Exit(list<FreeMemorySpace*> &freeMemorySpaceList);
};

ostream& operator<<(ostream& out, const Process &process);

#endif // PROCESS_H_INCLUDED
