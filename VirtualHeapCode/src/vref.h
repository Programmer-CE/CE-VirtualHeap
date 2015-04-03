#include "vobject.h"

#ifndef VREF_H
#define VREF_H

class vRef
{
    friend class vHeap;
    int _Id = -1;
    vRef(int pId,bool pDestroyRef);
    static bool MutexFlag;
    static void powerOffMutex();
    static void turnOnMutex();
public:
    bool _fromVHeap;
    bool _destroyRef;
    vRef(){}

    vRef(const void * pMemorySection);
    vRef(const vRef & pRef);
    vRef(const vRef * pRef);
    vRef &operator =(const vObject *pVObject);
    static vRef assing(size_t pSize, const vObject *pVObject);
    vRef &operator =(int pAddress);
    vRef &operator =(vRef pRef);
    vObject *operator * ();
    bool operator ==(const vRef & pVRef);
    unsigned int getWeight();
    std::string getType();
    virtual ~vRef();

};

#endif // VREF_H
