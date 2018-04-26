#ifndef VFS_H_INCLUDED
#define VFS_H_INCLUDED

#include <set>
#include "v-node.h"

class VFS
{
    vNode* root;
    vNode* currentVnode;
    set<vNode*, vNodeCompare> fileTable;
public:
    VFS();
    ~VFS();
    void ReadDriveData(Drive drives[]);
    void List(Drive drives[]);
    void ShowFolder(Drive drives[]);
    void OpenFolder(Drive drives[]);
    void AddFile(Drive drives[]);
    void DeleteFile(Drive drives[]);
    void PrintFile(Drive drives[]);
    void GoBack(Drive drives[]);
    void PrintMenu();
    void Start();
};

#endif // VFS_H_INCLUDED
