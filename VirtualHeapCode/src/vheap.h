#include "vref.h"
#include <string>
#include "ordinateList/doublelist.h"
#include "minimalismbitvector.h"
#include <cstdio>
#include <cstdlib>

using namespace std;

#ifndef VHEAP_H
#define VHEAP_H

class vHeap
{
    static vHeap *_Instance;
    void *_Chunk;
    DoubleList<MinimalismBitVector> _BitVector;
    vHeap();
public:
    static vHeap* getInstance();
    vRef *vMalloc(size_t pSize, string pType);
    void vFree(vRef* pRef);
    virtual ~vHeap()
    {
        delete _Instance;
        //delete _BitVector;
        free(_Chunk);
    }
};

#endif // VHEAP_H
