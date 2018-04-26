#ifndef I-NODE_H_INCLUDED
#define I-NODE_H_INCLUDED

#include <iostream>
#include <string>

using namespace std;

class iNode
{
    unsigned int id;
    bool type;
    unsigned int level;
    string fileName;
    string extension;
    string data;
    unsigned int fileNumber;
    iNode* files[5];
public:
    iNode();
    ~iNode(){};
    unsigned int GetId() const
    {
        return id;
    }
    void SetId(const unsigned int &newId)
    {
        id=newId;
    }
    bool GetType() const
    {
        return type;
    }
    void SetType(const bool &newType)
    {
        type=newType;
    }
    unsigned int GetLevel() const
    {
        return level;
    }
    void SetLevel(const unsigned int &newLevel)
    {
        level=newLevel;
    }
    string GetFileName() const
    {
        return fileName;
    }
    void SetFileName(const string &newFileName)
    {
        fileName=newFileName;
    }
    string GetExtension() const
    {
        return extension;
    }
    void SetExtension(const string &NewExtension)
    {
        extension=NewExtension;
    }
    string GetData() const
    {
        return data;
    }
    void SetData(const string &NewData)
    {
        data=NewData;
    }
    unsigned int GetFileNumber() const
    {
        return fileNumber;
    }
    void SetFileNumber(const unsigned int &newFileNumber)
    {
        fileNumber=newFileNumber;
    }
    iNode* GetFiles(const unsigned int &i) const
    {
        return files[i];
    }
    void SetFiles(const unsigned int &i, iNode* file)
    {
        files[i]=file;
    }
    friend ostream& operator<<(ostream& out, const iNode &node);
};

ostream& operator<<(ostream& out, const iNode &node);

struct iNodeCompare
{
    bool operator() (const iNode* node1, const iNode* node2)
    {
        return ((*node1).GetId()>(*node2).GetId());
    }
};

#endif // I-NODE_H_INCLUDED
