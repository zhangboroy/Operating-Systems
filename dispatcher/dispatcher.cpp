#include <cstdlib>
#include <ctime>
#include <fstream>
#include "dispatcher.h"

Dispatcher::Dispatcher()
{
    srand(time(0));
    ifstream inData;
    inData.open("process.txt");
    inData>>processNumber>>timeQuantum;
    processRemain=processNumber;
    running=0;
    process=new Process[processNumber];
    int id=0;
    string name="";
    int priority=0;
    int startTime=0;
    int burstTime=0;
    for (int i=0;i<processNumber;i++)
    {
        inData>>id>>name>>priority>>startTime>>burstTime;
        process[i].Set(id,name,priority,startTime,burstTime);
    }
    inData.close();
    outData.open("output.txt");
}

//output system status
void Dispatcher::OutputSystemStatus()
{
    //output the running process
    outData<<"The Running Process: ";

    if (running!=0)
    {
        outData<<(*running).GetName()<<"(ID: "<<(*running).GetId()<<")\n\n";
    }
    else
    {
        outData<<"\n\n";
    }

    //output the Ready Queue
    outData<<"The Ready Queues:\n";
    for (int i=0;i<5;++i)
    {
        outData<<"\tPriority "<<i<<":\tName\tID\n";
        for (list<Process*>::iterator j=readyQueue[i].begin();j!=readyQueue[i].end();++j)
        {
            outData<<"\t\t\t"<<(**j).GetName()<<'\t'<<(**j).GetId()<<endl;
        }
        outData<<endl;
    }

    //output the Blocked List
    outData<<"The Blocked List:\tName\tID\n";
    for (list<Process*>::iterator j=blockedList.begin();j!=blockedList.end();++j)
    {
        outData<<"\t\t\t"<<(**j).GetName()<<'\t'<<(**j).GetId()<<endl;
    }
    outData<<endl;
}

//check if there are new ready processes and add them into the ready queue
void Dispatcher::CheckReady(const int &time)
{
    for (int i=0;i<processNumber;++i)
    {
        if (process[i].GetStartTime()==time)
        {
            readyQueue[process[i].GetPriority()].push_back(&process[i]);
            outData<<"Process "<<process[i].GetName()<<"(ID: "<<process[i].GetId()<<") is added into the Ready Queue.\n\n";
            //output system status
            OutputSystemStatus();
        }
    }
}

//check if there is any process in the blocked list having finished waiting and add them into the ready queue
void Dispatcher::CheckBlocked()
{
    //a signal to show if there is a process in the blocked list having become unblocked
    bool unblocked=false;

    //check every process in the blocked list
    for (list<Process*>::iterator i=blockedList.begin();i!=blockedList.end();)
    {
        //there is a 10% chance that a process can become unblocked
        if (rand()%10<1)
        {
            readyQueue[(**i).GetPriority()].push_back(*i);
            outData<<"Process "<<(**i).GetName()<<"(ID: "<<(**i).GetId()<<") is moved from the Blocked List to the Ready Queue.\n\n";
            blockedList.erase(i++);
            unblocked=true;
        }
        else
        {
            ++i;
        }
    }
    //if there is a process in the blocked list having become unblocked
    if (unblocked)
    {
        //output system status
        OutputSystemStatus();
    }
}

//check if there is a running process, if not select 1
bool Dispatcher::CheckRunning()
{
    //if there is no process running
    if (running==0)
    {
        //select 1 process from the priority queue
        for (int i=0;i<5;++i)
        {
            //if this Ready Queue is not empty
            if (!readyQueue[i].empty())
            {
                running=readyQueue[i].front();
                readyQueue[i].pop_front();
                outData<<"Process "<<(*running).GetName()<<"(ID: "<<(*running).GetId()<<") is running.\n\n";
                //output system status
                OutputSystemStatus();
                return true;
            }
        }
        //if there is no process in the priority queue
        return false;
    }
    //if there is a process running
    else
    {
        return true;
    }
}

//start the dispatcher
void Dispatcher::Start()
{
    int processRunResult=0;
    int time=0;

    while (true)
    {
        //check if there are new ready processes and add them into the ready queue
        CheckReady(time);
        //check if there is any process in the blocked list having finished waiting and add them into the ready queue
        CheckBlocked();

        //check if there is a running process, if not select 1
        if (CheckRunning())
        {
            //process run
            processRunResult=(*running).Run(timeQuantum);
        }
        //if there is no process running
        else
        {
            //if there is no process left
            if (processRemain==0)
            {
                return;
            }
            //keep the system status
            else
            {
                processRunResult=3;
            }
        }

        //process is terminated
        if (processRunResult==0)
        {
            outData<<"Process "<<(*running).GetName()<<"(ID: "<<(*running).GetId()<<") is terminated.\n"<<endl;
            running=0;
            --processRemain;
            //output system status
            OutputSystemStatus();
        }
        //process is blocked
        else if (processRunResult==1)
        {
            outData<<"Process "<<(*running).GetName()<<"(ID: "<<(*running).GetId()<<") is blocked.\n"<<endl;
            blockedList.push_back(running);
            running=0;
            //output system status
            OutputSystemStatus();
        }
        //process is over its time Quantum
        else if (processRunResult==2)
        {
            outData<<"Process "<<(*running).GetName()<<"(ID: "<<(*running).GetId()<<") is over its time Quantum.\n"<<endl;
            readyQueue[(*running).GetPriority()].push_back(running);
            running=0;
            //output system status
            OutputSystemStatus();
        }
        ++time;
    }
}
