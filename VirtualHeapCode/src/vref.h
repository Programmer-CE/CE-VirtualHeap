#include "vobject.h"

#ifndef VREF_H
#define VREF_H

class vRef
{
    friend class vHeap;
    int _Id = -1;
public:
    vRef(const void * pMemorySection);
    vRef(int pId);
    vRef(const vObject * pObject);
    vRef(const vRef & pRef);
    vRef &operator =(const vObject *pVObject);
    static vRef assing(size_t pSize, const vObject *pVObject);
    //vRef &operator =(int pAddress);
    vObject *operator * ();
    bool operator ==(const vRef & pVRef);
    void *getDir();
    virtual ~vRef();

};

#endif // VREF_H
