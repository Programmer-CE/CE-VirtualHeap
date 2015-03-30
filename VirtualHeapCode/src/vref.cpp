#include "vref.h"
#include "vheap.h"
#include <typeinfo>
#include "nullpointerexception.h"
#include <iostream>



vRef::vRef(const void *pMemorySection)
{
    vHeap::getInstance()->get(this,pMemorySection);
}

vRef::vRef(int pId)
{
    _Id = pId;
}

vRef::vRef(const vObject *pObject)
{
    vHeap * tmp = vHeap::getInstance();
    tmp->set(this,pObject);
}

vRef::vRef(const vRef &pRef)
{
    vHeap::getInstance()->removeReference(this);
    this->_Id = vHeap::getInstance()->get(pRef._Id)._Id;
}

vRef &vRef::operator =(const vObject *pVObject)
{
    vHeap * tmp = vHeap::getInstance();
    tmp->set(this,pVObject);

    return *this;
}

vRef vRef::assing(size_t pSize,const vObject *pVObject)
{
    vRef t= (vHeap::getInstance())->vMalloc(pSize,pVObject->getType());
    vHeap::getInstance()->set(&t,pVObject);
    //t = pVObject;
    return t;
}
/**
vRef &vRef::operator =(int pAddress)
{
    vHeap::getInstance()->removeReference(this);
    *this = vHeap::getInstance()->get(pAddress);
    return *this;
}
*/

vObject *vRef::operator *()
{
    return vHeap::getInstance()->get(this);
}

bool vRef::operator ==(const vRef &pVRef)
{
    return _Id == pVRef._Id;
}


void *vRef::getDir()
{
    return vHeap::getInstance()->getDir(this);
}

vRef::~vRef()
{
    vHeap * tmp = vHeap::getInstance();
    tmp->removeReference(this);
}
