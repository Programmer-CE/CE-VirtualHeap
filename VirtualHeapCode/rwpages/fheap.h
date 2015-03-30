#ifndef FHEAP
#define FHEAP
#include <string>


class registry{
public:
    registry(int d,int a);
    int id;
    int id2;
};

class fHeap{
private:

    bool _isPage=false;
    int _memoryPage;
    int _totalData=0;
    int _totalDataMemory=0;
public:
    fHeap(int overWeight);
    void addToPage(registry nregistry);
    registry searchInPage(int id,int weight);
    void deletePage();
    ~fHeap(){}


};

#endif // FHEAP

