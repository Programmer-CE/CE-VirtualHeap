#include "vref.h"
#include "vheap.h"
#include <typeinfo>
#include "nullpointerexception.h"
#include <iostream>
#include <pthread.h>

//pthread_mutex_t mut2 = PTHREAD_MUTEX_INITIALIZER;

vRef::vRef(const void *pMemorySection)
{
    //vHeap::getInstance()->get(this,pMemorySection);
}

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

    try{
    vHeap::getInstance()->set(this,pVObject);
    }
    catch(NullPointerException e){
        std::cout << "ref error" << _Id << std::endl;
        exit (0);
    }

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


vRef vRef::assing(size_t pSize,const vObject *pVObject)
{
    vRef t(vHeap::getInstance()->vMalloc(pSize,pVObject->getType())._Id,true);
    vHeap::getInstance()->set(&t,pVObject);
    return t;
}

vObject *vRef::operator *()
{
    vObject * toReturn;
    try{
    vHeap::getInstance()->protect(this);
    toReturn = vHeap::getInstance()->get(this);
    }
    catch(NullPointerException e){
        std::cout << "ref error" << _Id << std::endl;
        exit (0);
    }
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

vRef::~vRef()
{
    if(_destroyRef){
        std::cout << "destroy ref " << _Id<< std::endl;
        vHeap::getInstance()->removeReference(this);
    }
}
