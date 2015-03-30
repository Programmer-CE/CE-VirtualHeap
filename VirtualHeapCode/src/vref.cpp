#include "vref.h"
#include "vheap.h"
#include <typeinfo>
#include "nullpointerexception.h"
#include <iostream>
#include <pthread.h>

bool vRef::MutexFlag = true;
//pthread_mutex_t mut2 = PTHREAD_MUTEX_INITIALIZER;

vRef::vRef(const void *pMemorySection)
{
    turnOnMutex();
    vHeap::getInstance()->get(this,pMemorySection);
    powerOffMutex();
}

vRef::vRef(int pId)
{
    _Id = pId;
}

vRef::vRef(const vObject *pObject)
{
    turnOnMutex();
    vHeap::getInstance();
    vHeap::getInstance()->set(this,pObject);
    powerOffMutex();
}

vRef::vRef(const vRef &pRef)
{
    turnOnMutex();
    vHeap::getInstance();
    vHeap::getInstance()->removeReference(this);
    this->_Id = vHeap::getInstance()->addRef(&pRef)._Id;
    //pRef._Id;//
    powerOffMutex();
}

void vRef::turnOnMutex()
{
    if (MutexFlag){
        pthread_mutex_lock(&vHeap::getInstance()->mut);
        MutexFlag = false;
    }
}

void vRef::powerOffMutex()
{
    if (MutexFlag){
        pthread_mutex_unlock(&vHeap::getInstance()->mut);
        MutexFlag=true;
    }
}

vRef &vRef::operator =(const vObject *pVObject)
{
    turnOnMutex();
    vHeap * tmp = vHeap::getInstance();
    tmp;
    tmp->set(this,pVObject);
    powerOffMutex();
    return *this;
}

vRef vRef::assing(size_t pSize,const vObject *pVObject)
{
    vRef t;
    turnOnMutex();
    vHeap::getInstance();
    t= (vHeap::getInstance())->vMalloc(pSize,pVObject->getType());
    vHeap::getInstance()->addRef(&t);
    vHeap::getInstance()->set(&t,pVObject);
    powerOffMutex();
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
    turnOnMutex();
    vObject * toReturn;
    toReturn = vHeap::getInstance()->get(this);
    powerOffMutex();
    return toReturn;
}

bool vRef::operator ==(const vRef &pVRef)
{
    return _Id == pVRef._Id;
}


void *vRef::getDir()
{
    turnOnMutex();
    void * toReturn;
    vHeap::getInstance();
    toReturn = vHeap::getInstance()->getDir(this);
    powerOffMutex();
    return toReturn;
}

unsigned int vRef::getWeight()
{
    unsigned int weight;
    turnOnMutex();
    weight = vHeap::getInstance()->getWight(this);
    powerOffMutex();
    return weight;
}

std::string vRef::getType()
{
    std::string type;
    turnOnMutex();
    type = vHeap::getInstance()->getType(this);
    powerOffMutex();
    return type;
}

vRef::~vRef()
{
    turnOnMutex();
    std::cout << "remove reference called, id " << _Id << std::endl;
    vHeap * tmp = vHeap::getInstance();
    tmp->removeReference(this);
    powerOffMutex();
}
