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
    List<int> _weightList;
public:
    fHeap(int overWeight);
    void addToPage(vRef newvRef);
    vRef searchInPage(int searchId);
    void deletePage();
    ~fHeap();


};

#endif // FHEAP

