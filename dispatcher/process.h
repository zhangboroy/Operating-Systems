#ifndef PROCESS_H_INCLUDED
#define PROCESS_H_INCLUDED

#include <string>

using namespace std;

class Process
{
    int id;
    string name;
    int priority;
    int startTime;
    int burstTime;
    int runTime;
    int runQuantum;
public:
    Process();
    ~Process(){};
    int GetId() const
    {
        return id;
    }
    string GetName() const
    {
        return name;
    }
    int GetPriority() const
    {
        return priority;
    }
    int GetStartTime() const
    {
        return startTime;
    }
    int GetBurstTime() const
    {
        return burstTime;
    }
    int GetRunTime() const
    {
        return runTime;
    }
    void Set(const int &id1, const string &name1, const int &priority1, const int &startTime1, const int &burstTime1);
    //run a process and return its state
    int Run(const int &timeQuantum);
};

#endif // PROCESS_H_INCLUDED
