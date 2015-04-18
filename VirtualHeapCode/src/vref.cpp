#include "vref.h"
#include "vheap.h"
#include <typeinfo>
#include "nullpointerexception.h"
#include <iostream>
#include <pthread.h>


vRef::vRef(int pId,bool pDestroyRef)
{
    _Id = pId;
    _destroyRef = pDestroyRef;
}

vRef::vRef(const vRef &pRef)
{
    vHeap::getInstance()->removeReference(this);
    vHeap::getInstance()->addRef(&pRef);
    this->_Id = pRef._Id;
    _destroyRef = pRef._destroyRef;
}


vRef &vRef::operator =(const vObject *pVObject)
{

    vHeap::getInstance()->set(this,pVObject);
    return *this;
}


vRef &vRef::operator =(vRef pRef)
{
    vHeap::getInstance()->removeReference(this);
    vHeap::getInstance()->addRef(&pRef);
    this->_Id = pRef._Id;
    _destroyRef = pRef._destroyRef;
    this->_Id = pRef._Id;
    return *this;
}



vRef vRef::reserve(size_t pSize)
{
    vRef t(vHeap::getInstance()->vMalloc(pSize)._Id,true);
    return t;
}


vObject *vRef::operator *()
{
    vObject * toReturn;
    vHeap::getInstance()->protect(this);
    toReturn = vHeap::getInstance()->get(this);
    return toReturn;
}

bool vRef::operator ==(const vRef &pVRef)
{
    return _Id == pVRef._Id;
}

unsigned int vRef::getWeight()
{
    unsigned int weight;
    weight = vHeap::getInstance()->getWight(this);
    return weight;
}

std::string vRef::getType()
{
    std::string type;
    type = vHeap::getInstance()->getType(this);
    return type;
}

bool vRef::isNull()
{
    try{
        vHeap::getInstance()->get(this);
        return true;
    }
    catch(NullPointerException e){
        return true;
    }
}

void vRef::leave()
{
    vHeap::getInstance()->unprotect(this);
}

vRef::~vRef()
{
    if(_destroyRef){
        vHeap::getInstance()->removeReference(this);
    }
}
