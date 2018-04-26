#include <fstream>
#include <iomanip>
#include "vfs.h"
#include "drive.h"

VFS::VFS()
{
    root=new vNode();
    fileTable.insert(root);
    currentVnode=root;
}

VFS::~VFS()
{
    vNode* temp=0;
    for (set<vNode*, vNodeCompare>::iterator i=fileTable.begin();i!=fileTable.end();++i)
    {
        temp=*i;
        delete temp;
        temp=0;
    }
    fileTable.clear();
}

void VFS::ReadDriveData(Drive drives[])
{
    ifstream inData;
    inData.open("driveData.txt");
    unsigned int recordNumber=0;
    inData>>recordNumber;
    int drive=0;
    unsigned int id=0;
    bool type=false;
    unsigned int level=0;
    string fileName="";
    string extension="";
    string data="";
    unsigned int fileNumber=0;
    int files[5]={0};
    iNode* newInode=0;
    vNode* newVnode=0;
    for (unsigned int j=0;j<3;++j)
    {
        newVnode=new vNode;
        (*newVnode).SetDrive('A'+j);
        (*newVnode).SetId(j+1);
        (*newVnode).SetInode(drives[j].FindNode(0));
        fileTable.insert(newVnode);
        newVnode=0;
    }

    for (unsigned int j=0;j<recordNumber;++j)
    {
        inData>>drive;
        newInode=new iNode;
        inData>>id;
        (*newInode).SetId(id);
        inData>>type;
        (*newInode).SetType(type);
        inData>>level;
        (*newInode).SetLevel(level);
        inData>>fileName;
        (*newInode).SetFileName(fileName);
        if (type)
        {
            inData>>fileNumber;
            (*newInode).SetFileNumber(fileNumber);
            for (unsigned int i=0;i<fileNumber;++i)
            {
                inData>>files[i];
                (*newInode).SetFiles(i, drives[drive].FindNode(files[i]));
            }
        }
        else
        {
            inData>>extension;
            (*newInode).SetExtension(extension);
            inData>>data;
            (*newInode).SetData(data);
        }

        drives[drive].Load(newInode);
        newVnode=new vNode;
        (*newVnode).SetDrive('A'+drive);
        (*newVnode).SetId(j+4);
        (*newVnode).SetInode(newInode);
        fileTable.insert(newVnode);
        newInode=0;
        newVnode=0;
    }
    inData.close();
}

void VFS::List(Drive drives[])
{
    cout<<"\nThe Actual File System Structure of VFS:\n";
    if (fileTable.empty())
    {
        cout<<"No Files!\n";
    }
    else
    {
        cout<<setw(16)<<"vNode\tFile Name\tExtension\tData\n";
        for (set<vNode*, vNodeCompare>::iterator i=fileTable.begin();i!=fileTable.end();++i)
        {
            cout<<(**i)<<endl;
        }
        cout<<endl;
    }
}

void VFS::ShowFolder(Drive drives[])
{
    if ((*currentVnode).GetDrive()==' ')
    {
        cout<<"\nIndex\tFile\n";
        for (int i=0;i<3;++i)
        {
            cout<<i<<'\t'<<(*drives[i].FindNode(0)).GetFileName()<<endl;
        }
    }
    else
    {
        cout<<"\nIndex\tFile\n";
        for (unsigned int i=0;i<(*(*currentVnode).GetInode()).GetFileNumber();++i)
        {
            cout<<i<<'\t'<<(*(*(*currentVnode).GetInode()).GetFiles(i)).GetFileName();
            if (!(*(*(*currentVnode).GetInode()).GetFiles(i)).GetType())
            {
                cout<<'.'<<(*(*(*currentVnode).GetInode()).GetFiles(i)).GetExtension();
            }
            cout<<endl;
        }
    }
}

void VFS::OpenFolder(Drive drives[])
{
    int index=0;
    cout<<"Please Enter the Index: "<<flush;
    cin>>index;

    if ((*currentVnode).GetDrive()==' ')
    {
        if (index<0 || index>2)
        {
            cout<<"\nInvalid Index!\n";
            return;
        }

        vNode temp;
        temp.SetId(index+1);
        currentVnode=*fileTable.find(&temp);
    }
    else
    {
        if (index<0 || index>=(*(*currentVnode).GetInode()).GetFileNumber())
        {
            cout<<"\nInvalid Index!\n";
            return;
        }
        else if (!(*(*(*currentVnode).GetInode()).GetFiles(index)).GetType())
        {
            cout<<"\nThis Is Not a Folder!\n";
            return;
        }

        for (set<vNode*, vNodeCompare>::iterator i=fileTable.begin();i!=fileTable.end();++i)
        {
            if ((**i).GetInode()==(*(*currentVnode).GetInode()).GetFiles(index))
            {
                currentVnode=*i;
                return;
            }
        }
    }
}

void VFS::AddFile(Drive drives[])
{
    if ((*currentVnode).GetDrive()==' ')
    {
        cout<<"\nPlease Select One of the Folders to Add File!\n";
        return;
    }
    else if ((*(*currentVnode).GetInode()).GetFileNumber()==5)
    {
        cout<<"\nThe Max File Number in a Single Folder Is 5!\n";
        return;
    }

    bool type=false;
    string fileName="";
    string extension="";
    string data="";
    cout<<"A Common File or A Folder (0: Common File  1: Folder)? "<<flush;
    cin>>type;
    if (!cin)
    {
        cin.clear();
        getline(cin,data);
        cout<<"\nInvalid Input!\n";
        return;
    }

    if (type && (*(*currentVnode).GetInode()).GetLevel()==3)
    {
        cout<<"\nThe Max Depth Is 4!\n";
        return;
    }

    getline(cin,fileName);
    cout<<"Please Enter the File Name: "<<flush;
    getline(cin,fileName);

    vNode* newFile=new vNode;
    (*newFile).SetDrive((*currentVnode).GetDrive());
    for (unsigned int i=1;;++i)
    {
        (*newFile).SetId(i);
        if (fileTable.find(newFile)==fileTable.end())
        {
            break;
        }
    }

    cout<<"\nThe Actual File System Structure of Drive "<<(*currentVnode).GetDrive()<<":\n";
    if (type)
    {
        (*newFile).SetInode(drives[(*newFile).GetDrive()-'A'].AddFile(fileName, "", "", type, (*currentVnode).GetInode()));
    }
    else
    {
        cout<<"Please Enter the Extension: "<<flush;
        getline(cin,extension);
        cout<<"Please Enter the Data: "<<flush;
        getline(cin,data);
        (*newFile).SetInode(drives[(*newFile).GetDrive()-'A'].AddFile(fileName, extension, data, type, (*currentVnode).GetInode()));
    }
    fileTable.insert(newFile);
    List(drives);
}

void VFS::DeleteFile(Drive drives[])
{
    if ((*currentVnode).GetDrive()==' ')
    {
        cout<<"\nYou Can Not Delete Folders!\n";
        return;
    }

    int index=0;
    cout<<"Please Enter the File Index: "<<flush;
    cin>>index;

    if (index<0 || index>=(*(*currentVnode).GetInode()).GetFileNumber())
    {
        cout<<"\nInvalid Index!\n";
        return;
    }

    if ((*(*(*currentVnode).GetInode()).GetFiles(index)).GetType())
    {
        cout<<"\nYou Can Not Delete Folders!\n";
        return;
    }

    for (set<vNode*, vNodeCompare>::iterator i=fileTable.begin();i!=fileTable.end();++i)
    {
        if ((**i).GetInode()==(*(*currentVnode).GetInode()).GetFiles(index))
        {
            vNode* toDelete=*i;
            fileTable.erase(i);
            delete toDelete;
            toDelete=0;
            break;
        }
    }

    cout<<"\nThe Actual File System Structure of Drive "<<(*currentVnode).GetDrive()<<":\n";
    drives[(*currentVnode).GetDrive()-'A'].DeleteFile(index, (*currentVnode).GetInode());
    List(drives);
}

void VFS::PrintFile(Drive drives[])
{
    if ((*currentVnode).GetDrive()==' ')
    {
        cout<<"\nYou Can Not Print Folders!\n";
        return;
    }

    int index=0;
    cout<<"Please Enter the File Index: "<<flush;
    cin>>index;

    if (index<0 || index>=(*(*currentVnode).GetInode()).GetFileNumber())
    {
        cout<<"\nInvalid Index!\n";
        return;
    }

    if ((*(*(*currentVnode).GetInode()).GetFiles(index)).GetType())
    {
        cout<<"\nYou Can Not Print Folders!\n";
        return;
    }

    drives[(*currentVnode).GetDrive()-'A'].PrintFile((*(*currentVnode).GetInode()).GetFiles(index));
}

void VFS::GoBack(Drive drives[])
{
    if (currentVnode==root)
    {
        cout<<"\nIt Is the Root Now!\n";
    }
    else if ((*(*currentVnode).GetInode()).GetId()==0)
    {
        currentVnode=root;
    }
    else
    {
        for (set<vNode*, vNodeCompare>::iterator i=fileTable.begin();i!=fileTable.end();++i)
        {
            if ((**i).GetInode()==0)
            {
                continue;
            }
            for (unsigned int j=0;j<(*(**i).GetInode()).GetFileNumber();++j)
            {
                if ((*(**i).GetInode()).GetFiles(j)==(*currentVnode).GetInode())
                {
                    cerr<<(*currentVnode).GetInode()<<'\t'<<(**i).GetInode()<<endl;
                    currentVnode=*i;
                    return;
                }
            }
        }
    }
}

void VFS::PrintMenu()
{
    cout<<"\nWelcome to VFS. Please Enter the Commands:\n"
        <<"1. Open a Folder\n"
        <<"2. Add a New File\n"
        <<"3. Delete a File\n"
        <<"4. Print a File\n"
        <<"5. Go Back\n"
        <<"6. Exit VFS\n";
}

void VFS::Start()
{
    cout<<left;
    Drive drives[3];
    ReadDriveData(drives);
    string choice="";

    do
    {
        ShowFolder(drives);
        PrintMenu();
        do
        {
            cout<<"Your Choice: "<<flush;
            cin>>choice;
        }
        while (choice!="1" && choice!="2" && choice!="3" && choice!="4" && choice!="5" && choice!="6");

        if (choice=="1")
        {
            OpenFolder(drives);
        }
        else if (choice=="2")
        {
            AddFile(drives);
        }
        else if (choice=="3")
        {
            DeleteFile(drives);
        }
        else if (choice=="4")
        {
            PrintFile(drives);
        }
        else if (choice=="5")
        {
            GoBack(drives);
        }
    }
    while (choice!="6");
}
