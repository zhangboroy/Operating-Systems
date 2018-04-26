#include "v-node.h"

vNode::vNode()
{
    drive=' ';
    id=0;
    inode=0;
}

ostream& operator<<(ostream& out, const vNode &node)
{
    out<<node.GetId();
    if (node.GetInode()!=0)
    {
        out<<'\t'<<*node.GetInode();
    }

    return out;
}
