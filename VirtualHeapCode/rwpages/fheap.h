#ifndef FHEAP
#define FHEAP
#include <string>
#include "src/vobject.h"


class fHeap{
private:

    bool _isPage=false;
    int _memoryPage;
    int _totalData=0;
    int _totalDataMemory=0;
public:
    fHeap(int overWeight);
    void addToPage(vObject newvObject,int Weight);
    vObject searchInPage(int searchId,int weight);
    void deletePage();
    ~fHeap();


};

#endif // FHEAP

