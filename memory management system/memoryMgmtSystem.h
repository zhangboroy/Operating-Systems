#ifndef MEMORYMGMTSYSTEM_H_INCLUDED
#define MEMORYMGMTSYSTEM_H_INCLUDED

#include "freeMemorySpace.h"

class MemoryMgmtSystem
{
    list<Process*> processList;
    list<FreeMemorySpace*> freeMemorySpaceList;
public:
    MemoryMgmtSystem();
    ~MemoryMgmtSystem();
    //print the main menu
    void PrintMenu() const;
    //print free memory space list
    void PrintFreeMemorySpaces();
    //print process status
    void PrintProcesses();
    //ask the user to choose actions from the menu and perform them
    void MainMenu(int& choice);
    //compact the free memory spaces
    void Compact();
    //start the system
    void Start();
};

#endif // MEMORYMGMTSYSTEM_H_INCLUDED
