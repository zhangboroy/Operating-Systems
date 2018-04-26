#ifndef DRIVE_H_INCLUDED
#define DRIVE_H_INCLUDED

#include <set>
#include "i-node.h"

class Drive
{
    static char name;
    set<iNode*, iNodeCompare> fileTable;
public:
    Drive();
    ~Drive();
    void Load(iNode* node);
    void List();
    iNode* AddFile(const string &fileName, const string &extension, const string &data, const bool &type, iNode* folder);
    void DeleteFile(const int &index, iNode* folder);
    void PrintFile(iNode* node);
    iNode* FindNode(const unsigned int &id);
};

#endif // DRIVE_H_INCLUDED
