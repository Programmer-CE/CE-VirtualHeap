#include "vheap.h"


vHeap* vHeap::_Instance = new vHeap();


vHeap::vHeap()
{
    //_BitVector = new DoubleList<MinimalismBitVector>();
    _Chunk = malloc(1024);
}

vHeap *vHeap::getInstance()
{
    if (!_Instance){
        _Instance = new vHeap();
    }
    return _Instance;
}
