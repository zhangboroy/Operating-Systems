#include <iomanip>
#include "drive.h"

char Drive::name='A';

Drive::Drive()
{
    iNode* root=new iNode();
    (*root).SetFileName(&name);
    ++name;
    fileTable.insert(root);
}

Drive::~Drive()
{
    iNode* temp=0;
    for (set<iNode*, iNodeCompare>::iterator i=fileTable.begin();i!=fileTable.end();++i)
    {
        temp=*i;
        delete temp;
        temp=0;
    }
    fileTable.clear();
}

void Drive::Load(iNode* node)
{
    fileTable.insert(node);
    if ((*node).GetLevel()==1)
    {
        iNode* root=FindNode(0);
        (*root).SetFiles((*root).GetFileNumber(), node);
        (*root).SetFileNumber((*root).GetFileNumber()+1);
        root=0;
    }
}

void Drive::List()
{
    if (fileTable.empty())
    {
        cout<<"No Files!\n";
    }
    else
    {
        cout<<setw(16)<<"iNode\tFile Name\tExtension\tData\n";
        for (set<iNode*, iNodeCompare>::iterator i=fileTable.begin();i!=fileTable.end();++i)
        {
            cout<<(**i).GetId()<<'\t'<<**i<<endl;
        }
    }
}

iNode* Drive::AddFile(const string &fileName, const string &extension, const string &data, const bool &type, iNode* folder)
{
    iNode* newFile=new iNode;
    for (unsigned int i=1;;++i)
    {
        (*newFile).SetId(i);
        if (fileTable.find(newFile)==fileTable.end())
        {
            break;
        }
    }
    (*newFile).SetType(type);
    (*newFile).SetLevel((*folder).GetLevel()+1);
    (*newFile).SetFileName(fileName);
    (*newFile).SetExtension(extension);
    (*newFile).SetData(data);

    fileTable.insert(newFile);

    (*folder).SetFiles((*folder).GetFileNumber(), newFile);
    (*folder).SetFileNumber((*folder).GetFileNumber()+1);
    List();

    return newFile;
}

void Drive::DeleteFile(const int &index, iNode* folder)
{
    set<iNode*, iNodeCompare>::iterator i=fileTable.find((*folder).GetFiles(index));
    (*folder).SetFileNumber((*folder).GetFileNumber()-1);
    for (int j=index;j<(*folder).GetFileNumber();++j)
    {
        (*folder).SetFiles(j, (*folder).GetFiles(j+1));
    }

    iNode* toDelete=*i;
    fileTable.erase(i);
    delete toDelete;
    toDelete=0;
    cout<<"\nFile Is Deleted.\n";

    List();
}

void Drive::PrintFile(iNode* node)
{
    set<iNode*, iNodeCompare>::iterator i=fileTable.find(node);
    cout<<"\nThe Content of "<<(**i).GetFileName()<<'.'<<(**i).GetExtension()<<":\n";
    cout<<(**i).GetData()<<endl;
}

iNode* Drive::FindNode(const unsigned int &id)
{
    iNode tempFile;
    tempFile.SetId(id);
    set<iNode*, iNodeCompare>::iterator i=fileTable.find(&tempFile);
    return *i;
}
