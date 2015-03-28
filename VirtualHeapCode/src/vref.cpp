#include "vref.h"
#include "vheap.h"
vRef::vRef(int * pId)
{
    _Id = pId;
}

vRef& vRef::operator =(vRef pVRef)
{
    this->_Id = pVRef._Id;
    return *this;
}

vRef &vRef::operator =(vObject *pVObject)
{
    vHeap::getInstance()->set(this,pVObject);
    return *this;
}

vRef &vRef::operator =(int pAddress)
{
    return vHeap::get(pAddress);
}

bool vRef::operator ==(const vRef &pVRef)
{
    return _Id == pVRef._Id;
}

vRef::~vRef()
{
    vHeap::getInstance()->removeReference(this);
}
