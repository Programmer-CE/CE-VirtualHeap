#include "vobject.h"

#ifndef VREF_H
#define VREF_H

class vRef
{
    friend class vHeap;
public:
    int _Id;
    vRef(int pId);
    vRef &operator =(vRef pVRef);
    vRef &operator =(vObject *pVObject);
    vRef &operator =(int pAddress);
    vObject &operator * ();
    bool operator ==(const vRef & pVRef);
    virtual ~vRef();

};

#endif // VREF_H
