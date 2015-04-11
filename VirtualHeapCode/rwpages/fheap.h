#ifndef FHEAP
#define FHEAP
#include <string>
#include "src/vobject.h"
#include "src/vref.h"
#include "list/List.h"
#include "list/Node.h"


class fHeap{
private:
    bool _isPage=false;
    int _memoryPage;
    int _totalData=0;
    int _totalDataMemory=0;
    char * _path;
    List<int> _weightList;
    void write(char * pToWrite, int pNumberOfCharacters);
    const char * read(int pFrom, int to);
public:
    fHeap(int overWeight, char* pPath);
    void paginate(char * pToWrite,int pNumberOfCharacters);
    const char * getPage(int pOffset, char * toCopy);
    void deletePage();
    ~fHeap();


};

#endif // FHEAP

