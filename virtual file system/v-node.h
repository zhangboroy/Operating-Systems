#ifndef V-NODE_H_INCLUDED
#define V-NODE_H_INCLUDED

#include "drive.h"

class vNode
{
    char drive;
    unsigned int id;
    iNode* inode;
public:
    vNode();
    ~vNode(){};
    char GetDrive() const
    {
        return drive;
    }
    void SetDrive(const char &newDrive)
    {
        drive=newDrive;
    }
    unsigned int GetId() const
    {
        return id;
    }
    void SetId(const unsigned int &newId)
    {
        id=newId;
    }
    iNode* GetInode() const
    {
        return inode;
    }
    void SetInode(iNode* newInode)
    {
        inode=newInode;
    }
    friend ostream& operator<<(ostream& out, const vNode &node);
};

ostream& operator<<(ostream& out, const vNode &node);

struct vNodeCompare
{
    bool operator() (const vNode* node1, const vNode* node2)
    {
        return ((*node1).GetId()>(*node2).GetId());
    }
};

#endif // V-NODE_H_INCLUDED
