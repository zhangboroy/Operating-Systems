#include <iomanip>
#include "i-node.h"

iNode::iNode()
{
    id=0;
    type=false;
    level=0;
    fileName="";
    extension="";
    data="";
    fileNumber=0;
    for (int i=0;i<5;++i)
    {
        files[i]=0;
    }
}

ostream& operator<<(ostream& out, const iNode &node)
{
    if (node.GetType())
    {
        out<<setw(16)<<node.fileName;
    }
    else
    {
        out<<setw(16)<<node.fileName<<setw(16)<<node.extension<<setw(16)<<node.data;
    }
    return out;
}
