#include <cstdlib>
#include "process.h"

Process::Process()
{
    id=0;
    name="";
    priority=0;
    startTime=0;
    burstTime=0;
    runTime=0;
    runQuantum=0;
}

void Process::Set(const int &id1, const string &name1, const int &priority1, const int &startTime1, const int &burstTime1)
{
    id=id1;
    name=name1;
    priority=priority1;
    startTime=startTime1;
    burstTime=burstTime1;
}

//run a process and return its state
int Process::Run(const int &timeQuantum)
{
    ++runTime;
    ++runQuantum;
    //process is terminated
    if (runTime==burstTime)
    {
        return 0;
    }
    //there is a 20% chance that a process becomes blocked
    else if (rand()%10<2)
    {
        runQuantum=0;
        return 1;
    }
    //process is over its time Quantum
    else if (runQuantum==timeQuantum)
    {
        runQuantum=0;
        return 2;
    }
    //process continue
    else
    {
        return 3;
    }
}
