#ifndef DISPATCHER_H_INCLUDED
#define DISPATCHER_H_INCLUDED

#include <list>
#include <fstream>
#include "process.h"

class Dispatcher
{
    int processNumber;
    int processRemain;
    int timeQuantum;
    Process* process;
    Process* running;
    list<Process*> readyQueue[5];
    list<Process*> blockedList;
    ofstream outData;
public:
    Dispatcher();
    ~Dispatcher()
    {
        delete []process;
        outData.close();
    };
    //output system status
    void OutputSystemStatus();
    //check if there are new ready processes and add them into the ready queue
    void CheckReady(const int &time);
    //check if there is any process in the blocked list having finished waiting and add them into the ready queue
    void CheckBlocked();
    //check if there is a running process, if not select 1
    bool CheckRunning();
    //start the dispatcher
    void Start();
};

#endif // DISPATCHER_H_INCLUDED
