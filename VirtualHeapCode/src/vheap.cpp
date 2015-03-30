#include "vheap.h"
#include "list/IIterator.h"
#include <cstring>
#include "fullmemoryexception.h"
#include "nullpointerexception.h"
#include "nomatchclassesexception.h"
#include "typeinfo"
vHeap* vHeap::_Instance = new vHeap();

// clase vacia que pesa 1 Byte
class ExplorationVHeap{

};

unsigned int vHeap::_Weight = 1024*1024;




int vHeap::searchBitVector(int pId)
{
    int left,center,right;
    left = 0;
    right = _BitVector->getLenght()-1;
    while(left <= right){
        center = int((left+right)/2);
        if (pId < _BitVector->get(center).getId()){
            right = center-1;
        }
        else if (pId > _BitVector->get(center).getId()){
            left = center+1;
        }
        else{
            return center;
        }
    }
    return -1;

}

vHeap *vHeap::getInstance()
{
    if (!_Instance){
        _Instance = new vHeap();
    }
    return _Instance;
}

vRef vHeap::vMalloc(unsigned int pSize, string pType)
{
    if (_CurrentMemoryUsed + pSize < _Weight + _OverWeight){
        IIterator<MinimalismBitVector> *iterator = _BitVector->getIterator();
        MinimalismBitVector toInsert;
        if (_BitVector->getLenght() > 0){
            MinimalismBitVector tmp1 = iterator->getNext();
            if (tmp1.Offset() > pSize){
                tmp1 = MinimalismBitVector(0,0,pType);
            }
            else if (_BitVector->getLenght() > 1){
                while(iterator->hasNext() && (iterator->getCurrent().Offset() - (tmp1.Offset()+tmp1.getWeight()) <= pSize)){
                    tmp1 = iterator->getNext();
                }
                if (!iterator->hasNext() && (iterator->getCurrent().Offset() - (tmp1.Offset()+tmp1.getWeight()) <= pSize)){
                    tmp1 = iterator->getCurrent();
                }
            }
            toInsert = MinimalismBitVector(tmp1.Offset() + tmp1.getWeight(),pSize,pType);


        }
        else{
            toInsert = MinimalismBitVector(0,pSize,pType);
        }
        toInsert.setReferenceCounter(1);
        toInsert.setOnUse(false);
        toInsert.setOnMemoryLocation(true);
        _BitVector->add(toInsert);
        _CurrentMemoryUsed += pSize;
        memset((int*)((ExplorationVHeap*)_Chunk + toInsert.Offset()),0,toInsert.getWeight());
        return vRef(toInsert.getId());
    }
    else{
        throw FullMemoryHeapException();
    }
}

void vHeap::vFree(vRef *pRef)
{
    int index = searchBitVector(pRef->_Id);
    if (index == -1)return;
    else{
        MinimalismBitVector memorylocation= _BitVector->get(index);
        memset((int*)((ExplorationVHeap*)_Chunk + memorylocation.Offset()),0,memorylocation.getWeight());
        _BitVector->remove(index);
    }
}

void vHeap::set(vRef *pRef, const vObject *pObject)
{
    int index = searchBitVector(pRef->_Id);
    if (index == -1){
        throw NullPointerException();
    }
    else{
        MinimalismBitVector & memorylocation= _BitVector->getReference(index);
        if (string(typeid(*pObject).name()) == memorylocation.getType()){
            copy((ExplorationVHeap*)pObject,
                 ((ExplorationVHeap*)pObject + memorylocation.getWeight()),
                 ((ExplorationVHeap*)_Chunk + memorylocation.Offset()));
            //memorylocation.addReference();
            //_BitVector->remove(index);
        }
        else{
            throw NoMatchClassesException();
        }
    }
}

vObject* vHeap::get(vRef *pRef)
{
    int index = searchBitVector(pRef->_Id);
    vObject * toReturn;
    if (index == -1){
        throw NullPointerException();
    }
    else{
        MinimalismBitVector & memorylocation= _BitVector->getReference(index);
        toReturn = (vObject*)((ExplorationVHeap*)_Chunk + memorylocation.Offset());
    }
    return toReturn;
}

void vHeap::get(vRef* pRef, const void *pAddress)
{
    int left,center,right;
    left = 0;
    right = _BitVector->getLenght()-1;
    while(left <= right){
        center = int((left+right)/2);
        if ((ExplorationVHeap*)pAddress < ((ExplorationVHeap*)_Chunk + _BitVector->get(center).Offset())){
            right = center-1;
        }
        else if ((ExplorationVHeap*)pAddress > ((ExplorationVHeap*)_Chunk + _BitVector->get(center).Offset())){
            left = center+1;
        }
        else{
            break;
        }
    }
    if (left > right){
        throw NullPointerException();
    }
    MinimalismBitVector & ref = _BitVector->getReference(center);
    ref.addReference();
    pRef->_Id = ref.getId();
}


vRef vHeap::get(int pId)
{
    int index = _BitVector->search(MinimalismBitVector(pId,1,string("")));
    if (index == -1){
        throw NullPointerException();
    }
    MinimalismBitVector &memorylocation = _BitVector->getReference(index);
    memorylocation.addReference();
    return vRef(memorylocation.getId());
}

vRef vHeap::addRef(const vRef *pRef)
{
    int index = searchBitVector(pRef->_Id);
    if (index == -1){
        throw NullPointerException();
    }
    MinimalismBitVector &memorylocation = _BitVector->getReference(index);
    memorylocation.addReference();
    //SE AGREGA otra referencia pues se destruye la referencia
    //del return cuando se libera la pila, esto hace
    //que se quite una referencia, por lo que hay que compensarlo
    //agregando una mas
    memorylocation.addReference();
    return vRef(memorylocation.getId());
}

bool vHeap::protect(vRef *pVRef)
{
    int index = searchBitVector(pVRef->_Id);
    if (index == -1){
        throw NullPointerException();
    }
    else{
        MinimalismBitVector &memorylocation = _BitVector->getReference(index);
        if (memorylocation.isOnUse()){
            return false;
        }
        else memorylocation.setOnUse(true);
    }
    return true;
}

void vHeap::removeReference(vRef *pVRef){
    int index = searchBitVector(pVRef->_Id);
    if (index == -1){
        return;
    }
    else{
        MinimalismBitVector &memorylocation = _BitVector->getReference(index);
        memorylocation.removeReference();
    }

}

void *vHeap::getDir(vRef *pVRef)
{
    void * toReturn;
    int index = searchBitVector(pVRef->_Id);
    if (index == -1){
        throw NullPointerException();
    }
    else{
        MinimalismBitVector & memorylocation= _BitVector->getReference(index);
        memorylocation.addReference();
        toReturn = (void*)((ExplorationVHeap*)_Chunk + memorylocation.Offset());
    }
    return toReturn;
}

void vHeap::collectGarbage()
{
    MinimalismBitVector memorylocation;
    for (int x = (_BitVector->getLenght()-1); x > -1;x--){
        memorylocation = _BitVector->get(x);
        if (memorylocation.ReferenceCounter() == 0){
            memset((int*)((ExplorationVHeap*)_Chunk + memorylocation.Offset()),0,memorylocation.getWeight());
            _BitVector->remove(x);
            std::cout << "borrado" << std::endl;

        }
    }
}

void vHeap::print()
{
    MinimalismBitVector m;
    std::cout << "tamanio: " << _BitVector->getLenght()
              <<  std::endl <<  "==========================" << std::endl;
    for (int x = 0; x < _BitVector->getLenght(); x++){
        m = _BitVector->get(x);
        std::cout << "id: "<< m.getId() << " type: " << m.getType() << " refcount: "<< m.ReferenceCounter() << std::endl;
    }
    std::cout << "==========================" << std::endl;
}

std::string vHeap::getType(vRef *pRef)
{

    MinimalismBitVector  memorylocation;
    int index = searchBitVector(pRef->_Id);
    if (index == -1){
        throw NullPointerException();
    }
    else{
        memorylocation= _BitVector->getReference(index);
    }
    return memorylocation.getType();
}

unsigned int vHeap::getWight(vRef *pRef)
{
    MinimalismBitVector memorylocation;
    int index = searchBitVector(pRef->_Id);
    if (index == -1){
        throw NullPointerException();
    }
    else{
        memorylocation= _BitVector->getReference(index);
    }
    return memorylocation.getWeight();
}
