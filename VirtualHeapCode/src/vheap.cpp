#include "vheap.h"
#include "list/IIterator.h"
#include <cstring>
#include "fullmemoryexception.h"
#include "nullpointerexception.h"
#include "nomatchclassesexception.h"
#include "typeinfo"

vHeap* vHeap::_Instance = 0;

pthread_mutex_t vHeap::mut = PTHREAD_MUTEX_INITIALIZER;
// clase vacia que pesa 1 Byte
class ExplorationVHeap{

};



unsigned int vHeap::_Weight = 1024*1024*50;




int vHeap::searchBitVector(int pId)
{
    for(int x = 0; x < _BitVector->getLenght();x++){
        if (_BitVector->get(x).getId() == pId){
            return x;
        }
    }
    return -1;

}

MinimalismBitVector vHeap::reallocMemory(const MinimalismBitVector *pFrom, unsigned int pAddressToAlloc)
{
    for (int x=0; x< pFrom->getWeight();x++){
        *((char*)_Chunk + pAddressToAlloc + x )= *((char*)_Chunk + pFrom->getOffset() + x );
    }
    MinimalismBitVector toReturn(*pFrom);
    toReturn.setOffset(pAddressToAlloc);
    return toReturn;
}

unsigned int vHeap::spaceBetween(MinimalismBitVector pRefa, MinimalismBitVector pRefb)
{
    if(pRefa.getOffset() > pRefb.getOffset()){
        return pRefa.getOffset() - (pRefb.getWeight() + pRefb.getOffset());
    }
    else if (pRefa.getOffset() < pRefb.getOffset()){
        return pRefb.getOffset() - (pRefa.getOffset() +pRefa.getWeight() );
    }
    return 0;
}

vHeap *vHeap::getInstance()
{
    pthread_mutex_lock(&vHeap::mut);
    if (!_Instance){
        std::cout << "New Instance Called" << std::endl;
        _Instance = new vHeap();
    }
    pthread_mutex_unlock(&vHeap::mut);
    return _Instance;
}

vRef vHeap::vMalloc(unsigned int pSize, string pType)
{
    pthread_mutex_lock(&vHeap::mut);
    if (_CurrentMemoryUsed + pSize < _Weight + _OverWeight){
        MinimalismBitVector toInsert;
        if (_BitVector->getLenght() > 0){
            MinimalismBitVector tmp1 = _BitVector->get(0);
            if (tmp1.getOffset() > pSize){
                tmp1 = MinimalismBitVector(0,0,pType);
            }
            else if (_BitVector->getLenght() > 1){
                for(int x = 1; x < _BitVector->getLenght();x++){
                    if (spaceBetween(_BitVector->get(x),tmp1) < pSize){
                        tmp1 = _BitVector->get(x);
                    }
                    else{
                        break;
                    }
                }
                /**
                if (!iterator->hasNext() && (iterator->getCurrent().Offset() - (tmp1.Offset()+tmp1.getWeight()) <= pSize)){
                    tmp1 = iterator->getCurrent();
                }
                */
            }

            toInsert = MinimalismBitVector(tmp1.getOffset() + tmp1.getWeight(),pSize,pType);


        }
        else{
            toInsert = MinimalismBitVector(0,pSize,pType);
        }
        toInsert.setReferenceCounter(2);
        toInsert.setOnUse(false);
        toInsert.setOnMemoryLocation(true);
        memset(((ExplorationVHeap*)_Chunk + toInsert.getOffset()),0,toInsert.getWeight());
        _CurrentMemoryUsed += pSize;
        _BitVector->add(toInsert);
        pthread_mutex_unlock(&vHeap::mut);
        return vRef(toInsert.getId(),true);
    }
    else{
        pthread_mutex_unlock(&vHeap::mut);
        throw FullMemoryHeapException();
    }
}

void vHeap::vFree(vRef *pRef)
{
    pthread_mutex_lock(&vHeap::mut);
    int index = searchBitVector(pRef->_Id);
    if (index == -1){}
    else{
        MinimalismBitVector memorylocation= _BitVector->get(index);
        memset((int*)((ExplorationVHeap*)_Chunk + memorylocation.getOffset()),0,memorylocation.getWeight());
        _CurrentMemoryUsed -=memorylocation.getWeight();
        _BitVector->remove(index);
    }
    pthread_mutex_unlock(&vHeap::mut);
}

void vHeap::set(vRef *pRef, const vObject *pObject)
{
    pthread_mutex_lock(&vHeap::mut);
    int index = searchBitVector(pRef->_Id);
    if (index == -1){
        pthread_mutex_unlock(&vHeap::mut);
        throw NullPointerException();
    }
    else{
        MinimalismBitVector  memorylocation= _BitVector->get(index);
        if (string(typeid(*pObject).name()) == memorylocation.getType()){
            copy((ExplorationVHeap*)pObject,
                 ((ExplorationVHeap*)pObject + memorylocation.getWeight()),
                 ((ExplorationVHeap*)_Chunk + memorylocation.getOffset()));
        }
        else{
            pthread_mutex_unlock(&vHeap::mut);
            throw NoMatchClassesException();
        }
    }
    pthread_mutex_unlock(&vHeap::mut);
}

vObject* vHeap::get(vRef *pRef)
{
    pthread_mutex_lock(&vHeap::mut);
    int index = searchBitVector(pRef->_Id);
    vObject * toReturn;
    if (index == -1){
        pthread_mutex_unlock(&vHeap::mut);
        throw NullPointerException();
    }
    else{
        MinimalismBitVector  memorylocation= _BitVector->get(index);
        toReturn = (vObject*)((ExplorationVHeap*)_Chunk + memorylocation.getOffset());
    }
    pthread_mutex_unlock(&vHeap::mut);
    return toReturn;
}

void vHeap::addRef(const vRef *pRef)
{
    pthread_mutex_lock(&vHeap::mut);
    int index = searchBitVector(pRef->_Id);
    if (index != -1){
        MinimalismBitVector  memorylocation= _BitVector->get(index);
        if(memorylocation.getReferenceCounter() > 0){
            memorylocation.addReference();
            _BitVector->remove(index);
            _BitVector->add(memorylocation);
        }
    }
    pthread_mutex_unlock(&vHeap::mut);
}

bool vHeap::protect(vRef *pVRef)
{
    pthread_mutex_lock(&vHeap::mut);
    int index = searchBitVector(pVRef->_Id);
    if (index == -1){
        pthread_mutex_unlock(&vHeap::mut);
        throw NullPointerException();
    }
    else{
        MinimalismBitVector memorylocation = _BitVector->get(index);
        if (memorylocation.isOnUse()){
            pthread_mutex_unlock(&vHeap::mut);
            return false;
        }
        else{
            memorylocation.setOnUse(true);
            _BitVector->remove(index);
            _BitVector->add(memorylocation);
        }
    }
    pthread_mutex_unlock(&vHeap::mut);
    return true;
}

void vHeap::removeReference(vRef *pVRef){
    pthread_mutex_lock(&vHeap::mut);
    int index = searchBitVector(pVRef->_Id);
    if (index == -1){
        std::cout << "no se encontro la referencias " << pVRef->_Id <<  " largo de referencias: " << _BitVector->getLenght() << " cantidad de memoria usada: " << _CurrentMemoryUsed << std::endl;
    }
    else{
        MinimalismBitVector memorylocation = _BitVector->get(index);
        if (memorylocation.getReferenceCounter() >0){
            memorylocation.removeReference();
            _BitVector->remove(index);
            _BitVector->add(memorylocation);
        }
    }
    pthread_mutex_unlock(&vHeap::mut);

}

void vHeap::collectGarbage()
{
    pthread_mutex_lock(&vHeap::mut);
    MinimalismBitVector *memorylocation;
    for (int x = _BitVector->getLenght()-1; x > -1;x--){
        memorylocation = &_BitVector->getReference(x);
        if (memorylocation->getReferenceCounter() == 0){
            memset(((ExplorationVHeap*)_Chunk + memorylocation->getOffset()),0,memorylocation->getWeight());
            _CurrentMemoryUsed -=memorylocation->getWeight();
            _BitVector->remove(x);
            break;

        }
    }
    pthread_mutex_unlock(&vHeap::mut);
}

void vHeap::compact()
{
    pthread_mutex_lock(&vHeap::mut);
    if (_BitVector->getLenght() > 0){
        MinimalismBitVector tmp1 = _BitVector->get(0);
        unsigned int space;
        MinimalismBitVector tmp2;
        MinimalismBitVector tmp3;
        for(int bitvectoractual = 1; bitvectoractual < _BitVector->getLenght();bitvectoractual++){
            tmp2=_BitVector->get(bitvectoractual);
            space= spaceBetween(tmp2,tmp1);
            if (space > 0){
                for(int otherbitvector = bitvectoractual;otherbitvector <_BitVector->getLenght();otherbitvector++){
                    tmp3 = _BitVector->get(otherbitvector);
                    if(!tmp3.isOnUse() && tmp3.getReferenceCounter() > 0 ){
                        if (space >= tmp3.getWeight()){
                            _BitVector->remove(otherbitvector);
                            reallocMemory(&tmp3,tmp1.getWeight() + tmp1.getOffset());
                            tmp3.setOffset(tmp1.getOffset() + tmp1.getWeight());
                            _BitVector->add(tmp3);
                            bitvectoractual++;
                            tmp1 = tmp3;
                        }
                    }
                }
                if (tmp1!= tmp3){
                    tmp1 = tmp2;
                }
            }
            else{
                tmp1 = tmp2;
            }
        }
    }
    pthread_mutex_unlock(&vHeap::mut);
}

void vHeap::print()
{
    pthread_mutex_lock(&vHeap::mut);
    MinimalismBitVector m;
    std::cout << "tamanio: " << _BitVector->getLenght()
              <<  std::endl <<  "==========================" << std::endl;
    for (int x = 0; x < _BitVector->getLenght(); x++){
        m = _BitVector->get(x);
        std::cout << "id: "<< m.getId() << " type: " << m.getType() << " refcount: "<< m.getReferenceCounter() << " offset: " << m.getOffset() << std::endl;
    }
    std::cout << "==========================" << std::endl;
    pthread_mutex_unlock(&vHeap::mut);
}

std::string vHeap::getType(vRef *pRef)
{
    pthread_mutex_lock(&vHeap::mut);
    MinimalismBitVector  memorylocation;
    int index = searchBitVector(pRef->_Id);
    if (index == -1){
        pthread_mutex_unlock(&vHeap::mut);
        throw NullPointerException();
    }
    else{
        memorylocation= _BitVector->get(index);
    }
    pthread_mutex_unlock(&vHeap::mut);
    return memorylocation.getType();
}

unsigned int vHeap::getWight(vRef *pRef)
{
    pthread_mutex_lock(&vHeap::mut);
    MinimalismBitVector memorylocation;
    int index = searchBitVector(pRef->_Id);
    if (index == -1){
        pthread_mutex_unlock(&vHeap::mut);
        throw NullPointerException();
    }
    else{
        memorylocation= _BitVector->get(index);
    }
    pthread_mutex_unlock(&vHeap::mut);
    return memorylocation.getWeight();
}
