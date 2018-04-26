#include <string>
#include "memoryMgmtSystem.h"

MemoryMgmtSystem::MemoryMgmtSystem()
{
    FreeMemorySpace* freeMemorySpace=new FreeMemorySpace(0, memorySize);
    freeMemorySpaceList.push_back(freeMemorySpace);
}

MemoryMgmtSystem::~MemoryMgmtSystem()
{
    //destroy the freeMemorySpaceList
    FreeMemorySpace* temp=0;
    while (!freeMemorySpaceList.empty())
    {
        temp=freeMemorySpaceList.front();
        freeMemorySpaceList.pop_front();
        delete temp;
        temp=0;
    }
    //destroy the processList
    Process* temp2=0;
    while (!processList.empty())
    {
        temp2=processList.front();
        processList.pop_front();
        delete temp2;
        temp2=0;
    }
}

//print the main menu
void MemoryMgmtSystem::PrintMenu() const
{
    cout<<"\nWelcome to the MemoryMgmtSystem. Please Enter the Commands:\n"
        <<"1. Show the process status\n"
        <<"2. Create a process\n"
        <<"3. Terminate a process\n"
        <<"4. Compact the free memory spaces\n"
        <<"5. Exit the system\n";
}

//print free memory space list
void MemoryMgmtSystem::PrintFreeMemorySpaces()
{
    cout<<endl;
    //no free space
    if (freeMemorySpaceList.empty())
    {
        cout<<"There is no free memory!\n";
    }
    //print free spaces 1 by 1
    else
    {
        for (list<FreeMemorySpace*>::iterator i=freeMemorySpaceList.begin();i!=freeMemorySpaceList.end();++i)
        {
            cout<<**i<<endl;
        }
    }
}

//print process status
void MemoryMgmtSystem::PrintProcesses()
{
    cout<<endl;
    //no processes
    if (processList.empty())
    {
        cout<<"There is no process running!\n";
    }
    //print processes 1 by 1
    else
    {
        for (list<Process*>::iterator i=processList.begin();i!=processList.end();++i)
        {
            cout<<**i<<endl;
        }
    }
}

//ask the user to choose actions from the menu and perform them
void MemoryMgmtSystem::MainMenu(int& choice)
{
    string garbage="";
    //only 1~5 are available
    while (choice!=1 && choice!=2 && choice!=3 && choice!=4 && choice!=5)
    {
        //ask the user to enter commands from the menu
        choice=0;
        cout<<"Your choice: "<<flush;
        cin>>choice;
        if (!cin)
        {
            cin.clear();
            getline(cin,garbage);
        }
        //1 = print process status
        if (choice==1)
        {
            PrintProcesses();
        }
        //2 = create a process
        else if (choice==2)
        {
            //ask the user to enter the size of the process
            int size=0;
            //size must > 0
            while (size<1)
            {
                cout<<"\nPlease enter the size of the process: "<<flush;
                cin>>size;
                if (!cin)
                {
                    cin.clear();
                    getline(cin,garbage);
                    size=0;
                }
            }
            //try to create a new process with certain size
            Process* newProcess=new Process(processList, freeMemorySpaceList, size);
            //if no enough memory space
            if ((*newProcess).GetBase()==-1)
            {
                //cancel and compact the free memory spaces
                delete newProcess;
                newProcess=0;
                cout<<"\nCompacting the free memory spaces...\n";
                Compact();
                //try again
                newProcess=new Process(processList, freeMemorySpaceList, size);
            }

            //if still no enough memory spaces
            if ((*newProcess).GetBase()==-1)
            {
                //cancel
                delete newProcess;
                newProcess=0;
            }
            //add the process to the running list
            else
            {
                processList.push_back(newProcess);
                //print free memory space list
                PrintFreeMemorySpaces();
            }
        }
        //3 = terminate a process
        else if (choice==3)
        {
            //if no process is running
            if (processList.empty())
            {
                cout<<"\nThere is no process running!\n";
            }
            else
            {
                //print process status
                PrintProcesses();
                int index=0;
                Process* processToBeTerminated=0;
                //ask the user to enter the ID of the process to be terminated, enter -1 to go back
                while (processToBeTerminated==0)
                {
                    cout<<"\nPlease choose which process to terminate (Enter -1 to go back): "<<flush;
                    cin>>index;
                    //-1 means go back
                    if (index==-1)
                    {
                        break;
                    }
                    if (!cin)
                    {
                        cin.clear();
                        getline(cin,garbage);
                        index=0;
                        continue;
                    }
                    //check if the ID is legal and terminate that one
                    for (list<Process*>::iterator i=processList.begin();i!=processList.end();++i)
                    {
                        if ((**i).GetID()==index)
                        {
                            processToBeTerminated=*i;
                            //remove it from the process list
                            processList.erase(i);
                            cout<<"\nProcess(ID: "<<(*processToBeTerminated).GetID()<<") has been terminated\n";
                            //terminate the process and free the space
                            (*processToBeTerminated).Exit(freeMemorySpaceList);
                            //destroy the process
                            delete processToBeTerminated;
                            processToBeTerminated=0;
                            //print free memory space list
                            PrintFreeMemorySpaces();
                            return;
                        }
                    }
                }
            }
        }
        //4 = compact free spaces
        else if (choice==4)
        {
            //compact the free memory spaces
            Compact();
        }
    }
}

//compact the free memory spaces
void MemoryMgmtSystem::Compact()
{
    //if there are free spaces
    if (!freeMemorySpaceList.empty())
    {
        //move the space to the front of the memory
        FreeMemorySpace* compactedMemorySpace=freeMemorySpaceList.front();
        (*compactedMemorySpace).ResetBase();
        //merge the free spaces 1 by 1 from the back of the list
        FreeMemorySpace* toBeDelete=0;
        while (freeMemorySpaceList.size()>1)
        {
            toBeDelete=freeMemorySpaceList.back();
            (*compactedMemorySpace).Grow((*toBeDelete).GetSize());
            freeMemorySpaceList.pop_back();
            delete toBeDelete;
            toBeDelete=0;
        }

        //put the processes right after the free space 1 by 1
        int newBase=(*compactedMemorySpace).GetSize();
        for (list<Process*>::iterator i=processList.begin();i!=processList.end();++i)
        {
            (**i).SetBase(newBase);
            newBase+=(**i).GetLimit();
        }
        cout<<"\nThe memory spaces have been compacted\n";
    }
    //print free memory space list
    PrintFreeMemorySpaces();
}

//start the system
void MemoryMgmtSystem::Start()
{
    int choice=0;
    //stop only if the user choose to exit by entering 5
    while (choice!=5)
    {
        choice=0;
        //print the main menu
        PrintMenu();
        //ask the user to choose actions from the menu and perform them
        MainMenu(choice);
    }
}
