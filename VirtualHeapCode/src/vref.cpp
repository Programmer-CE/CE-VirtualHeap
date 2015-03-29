#include "vref.h"
#include "vheap.h"
vRef::vRef(int pId)
{
    _Id = pId;
}

vRef& vRef::operator =(vRef pVRef)
{
    vHeap::getInstance()->removeReference(this);
    this->_Id = vHeap::getInstance()->get(pVRef._Id)._Id;
    return *this;
}

vRef &vRef::operator =(vObject *pVObject)
{
    vHeap * tmp = vHeap::getInstance();
    tmp->removeReference(this);
    tmp->set(this,pVObject);
    return *this;
}

vRef &vRef::operator =(int pAddress)
{
    vHeap::getInstance()->removeReference(this);
    *this = vHeap::getInstance()->get(pAddress);
    return *this;
}

vObject &vRef::operator *()
{
    return *vHeap::getInstance()->get(this);
}

bool vRef::operator ==(const vRef &pVRef)
{
    return _Id == pVRef._Id;
}

vRef::~vRef()
{
    vHeap * tmp = vHeap::getInstance();
    tmp->removeReference(this);
}
