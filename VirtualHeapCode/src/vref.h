#include "vobject.h"

#ifndef VREF_H
#define VREF_H

class vRef
{
    friend class vHeap;
    int _Id = -1;
    vRef(int pId);
    static bool MutexFlag;
    static void powerOffMutex();
    static void turnOnMutex();
public:
    vRef(){}

    vRef(const void * pMemorySection);
    vRef(const vObject * pObject);
    vRef(const vRef & pRef);
    vRef &operator =(const vObject *pVObject);
    static vRef assing(size_t pSize, const vObject *pVObject);
    //vRef &operator =(int pAddress);
    vObject *operator * ();
    bool operator ==(const vRef & pVRef);
    void *getDir();
    unsigned int getWeight();
    std::string getType();
    virtual ~vRef();

};

#endif // VREF_H
