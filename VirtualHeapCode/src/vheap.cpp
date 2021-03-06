#include "vheap.h"
#include "list/IIterator.h"
#include <cstring>
#include "fullmemoryexception.h"
#include "nullpointerexception.h"
#include "nomatchclassesexception.h"
#include "typeinfo"
#include "arraysizenocompatible.h"
#include "indexoutbounds.h"
#include <sstream>
#include <time.h>
#include <sys/time.h>




// clase vacia que pesa 1 Byte
class ExplorationVHeap{

};


string intToString(int toConvert){
    ostringstream converter;
    converter << toConvert;
    return converter.str();
}



/*
 *Instancias Estasticas
 *============================================================
 */

vHeap* vHeap::_Instance = 0;

pthread_mutex_t vHeap::mut = PTHREAD_MUTEX_INITIALIZER;


//unsigned int vHeap::_Weight = 1024*1024*50;
unsigned int vHeap::_Weight = 512;

/*
 *Fin Instancias Estasticas
 *============================================================
 */






/*
 *Metodos De acceso al vHeap
 *============================================================
 */

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







vRef vHeap::vMalloc(unsigned int pSize)
{
    pthread_mutex_lock(&vHeap::mut);
    if(_vDebug)comienzo = time(NULL);
    _vMallocCalled++;
    if (_CurrentMemoryUsed + pSize <= _Weight){
        // inserta en memoria fisica
        _NotPagedObject++;
        return insertOnPhysicalMemory(pSize);
    }
    else if (_CurrentMemoryUsed + pSize < _Weight + _OverWeight){
        // inserta en memoria fisica y baja un objeto a memoria de paginacion
        _PagedObject++;
        return insertOnDiskMemory(pSize);
    }
    else{
        //arroja error si la memoria esta llena del todo
        if(_vDebug)final = time(NULL);
        pthread_mutex_unlock(&vHeap::mut);
        throw FullMemoryHeapException();
    }
}

void vHeap::calculateTime()
{
    if(_vDebug)final = time(NULL);
}





void vHeap::vFree(vRef *pRef)
{
    pthread_mutex_lock(&vHeap::mut);
    if(_vDebug)comienzo=time(NULL);
    _vFreeCalled++;
    int index = searchBitVector(pRef->_Id);
    if (index == -1);//no hace nada si existe
    else{
        //encuentra el objeto y lo borra, ademas de setear sus datos en ceros
        MinimalismBitVector memorylocation= _BitVector->get(index);
        if (memorylocation.isChargedOnMemory()){
            memset((int*)((ExplorationVHeap*)_Chunk + memorylocation.getOffset()),0,memorylocation.getWeight());
        }
        else {
            char tmp[memorylocation.getWeight()];
            _HeapOfPage.getPage(memorylocation.getOffset()-_Weight,tmp);
            moveInPlaceOffset(index,memorylocation.getWeight());
            calculateTime();
            pthread_mutex_unlock(&vHeap::mut);
            throw FullMemoryHeapException();
        }
        _CurrentMemoryUsed -=memorylocation.getWeight();
        _BitVector->remove(index);
    }
    calculateTime();
    pthread_mutex_unlock(&vHeap::mut);
}








void vHeap::set(vRef *pRef, const vObject *pObject)
{
    pthread_mutex_lock(&vHeap::mut);
    if(_vDebug)comienzo=time(NULL);
    int index = searchBitVector(pRef->_Id);
    if (index == -1){
        //arroja un error en el caso de que la referencia no exista
        calculateTime();
        pthread_mutex_unlock(&vHeap::mut);
        throw NullPointerException();
    }
    else{
        //si exite entonces:
        MinimalismBitVector  *memorylocation= &_BitVector->getReference(index);
        vObject * toReturn;
        if (!memorylocation->isChargedOnMemory()){
            int index2 = getIndexToPagine(memorylocation->getWeight());
            if (index2 == -1){
                calculateTime();
                pthread_mutex_unlock(&vHeap::mut);
                throw FullMemoryHeapException();
            }
            MinimalismBitVector tmp = _BitVector->get(index2);
            toReturn= (vObject*)((ExplorationVHeap*)_Chunk + tmp.getOffset());
            char chainInterprate[tmp.getWeight()];
            for (int x = 0; x < tmp.getWeight();x++){
                chainInterprate[x] = *((char*) (&toReturn) + x);
            }
            int offset = tmp.getOffset();
            int weight = memorylocation->getWeight() - tmp.getWeight();
            memset(((ExplorationVHeap*)_Chunk + tmp.getOffset()),0,tmp.getWeight());
            tmp.setOffset(_Weight + _HeapOfPage.getMemoryUsed());
            _BitVector->remove(index2);
            MinimalismBitVector tmp2 = *memorylocation;
            _BitVector->remove(index);
            tmp2.setOffset(offset);
            _BitVector->add(tmp2);
            memorylocation= &_BitVector->getReference(index2);
            _BitVector->add(tmp);

            moveInPlaceOffset(index2,weight);
            _HeapOfPage.paginate(chainInterprate,tmp.getWeight());
            calculateTime();
            pthread_mutex_unlock(&vHeap::mut);
            throw FullMemoryHeapException();

        }
        else toReturn= (vObject*)((ExplorationVHeap*)_Chunk + memorylocation->getOffset());

        //comparar los tipos si son iguales entonces:
        if(pObject->getType() == toReturn->getType()){

            //copiar el objeto por medio del metodo copy de la biblioteca cstring
            copy((ExplorationVHeap*)pObject,
                 ((ExplorationVHeap*)pObject + memorylocation->getWeight()),
                 ((ExplorationVHeap*)_Chunk + memorylocation->getOffset()));
        }
        else{
            //si no comparar si los tipo de dato son ambos arrays
            if (pObject->isArray() && toReturn->isArray()){
                // si ambos son arrays se arroja el error de
                //tamanios incompatibles esto para que le sea mas facil
                //al programador encontrar errores
                if (pObject->getNumOfObjects() != toReturn->getNumOfObjects()){
                    pthread_mutex_unlock(&vHeap::mut);
                    throw ArraySizeNoCompatible();
                }
            }
            pthread_mutex_unlock(&vHeap::mut);
            throw NoMatchClassesException();
        }
    }
    calculateTime();
    pthread_mutex_unlock(&vHeap::mut);
}







vObject* vHeap::get(vRef *pRef)
{
    pthread_mutex_lock(&vHeap::mut);
    if(_vDebug)comienzo=time(NULL);
    int index = searchBitVector(pRef->_Id);
    vObject * toReturn;
    if (index == -1){
        //se arroja un nullpointer exception si el objeto no existe
        calculateTime();
        pthread_mutex_unlock(&vHeap::mut);
        throw NullPointerException();
    }
    else{
        //si el objeto existe entonces retornar un puntero apuntando a la zona de memoria
        //en la que se encuentra el objeto
        MinimalismBitVector  memorylocation= _BitVector->get(index);
        if (!memorylocation.isChargedOnMemory()){
            int index2 = getIndexToPagine(memorylocation.getWeight());
            if (index2 == -1){
                calculateTime();
                pthread_mutex_unlock(&vHeap::mut);
                throw FullMemoryHeapException();
            }
            MinimalismBitVector tmp = _BitVector->get(index2);
            toReturn= (vObject*)((ExplorationVHeap*)_Chunk + tmp.getOffset());
            char chainInterprate[tmp.getWeight()];
            for (int x = 0; x < tmp.getWeight();x++){
                chainInterprate[x] = *((char*) (&toReturn) + x);
            }
            int offset = tmp.getOffset();
            int weight = memorylocation.getWeight() - tmp.getWeight();
            memset(((ExplorationVHeap*)_Chunk + tmp.getOffset()),0,tmp.getWeight());
            tmp.setOffset(_Weight + _HeapOfPage.getMemoryUsed());
            _BitVector->remove(index2);
            MinimalismBitVector tmp2 = memorylocation;
            _BitVector->remove(index);
            tmp2.setOffset(offset);
            _BitVector->add(tmp2);
            memorylocation= _BitVector->get(index2);
            _BitVector->add(tmp);

            moveInPlaceOffset(index2,weight);
            _HeapOfPage.paginate(chainInterprate,tmp.getWeight());
            calculateTime();
            pthread_mutex_unlock(&vHeap::mut);
            throw FullMemoryHeapException();
        }
        toReturn = (vObject*)((ExplorationVHeap*)_Chunk + memorylocation.getOffset());
    }
    calculateTime();
    pthread_mutex_unlock(&vHeap::mut);
    return toReturn;
}







void vHeap::addRef(const vRef *pRef)
{
    pthread_mutex_lock(&vHeap::mut);
    if(_vDebug)comienzo=time(NULL);
    int index = searchBitVector(pRef->_Id);
    if (index != -1){
        // si la referencia existe entonces, buscar el objeto
        MinimalismBitVector  memorylocation= _BitVector->get(index);
        // si la cantidad de referencias es mayor a cero entonces
        // se agrega una referencia mas al objeto
        // esto con la finalidad de evitar futuros errores con
        // el garbage collector
        if(memorylocation.getReferenceCounter() > 0){
            memorylocation.addReference();
            _BitVector->remove(index);
            _BitVector->add(memorylocation);
        }
    }
    calculateTime();
    pthread_mutex_unlock(&vHeap::mut);
}






bool vHeap::protect(vRef *pVRef)
{
    pthread_mutex_lock(&vHeap::mut);
    if(_vDebug)comienzo=time(NULL);
    int index = searchBitVector(pVRef->_Id);
    if (index == -1){
        /*
         * Si la referencia es invalida entonces se arroja un error
         */
        calculateTime();
        pthread_mutex_unlock(&vHeap::mut);
        throw NullPointerException();
    }
    else{
        /*
         * So la referencia es valida entonces debe bloquear el objeto
         * de ser posible
         */
        MinimalismBitVector memorylocation = _BitVector->get(index);
        if (memorylocation.isOnUse()){
            calculateTime();
            pthread_mutex_unlock(&vHeap::mut);
            return false;
        }
        else{
            memorylocation.setOnUse(true);
            _BitVector->remove(index);
            _BitVector->add(memorylocation);
        }
    }
    calculateTime();
    pthread_mutex_unlock(&vHeap::mut);
    return true;
}

bool vHeap::unprotect(vRef *pVRef)
{
    pthread_mutex_lock(&vHeap::mut);
    if(_vDebug)comienzo=time(NULL);
    int index = searchBitVector(pVRef->_Id);
    if (index == -1){
        /*
         * Si la referencia es invalida entonces se arroja un error
         */
        calculateTime();
        pthread_mutex_unlock(&vHeap::mut);
        throw NullPointerException();
    }
    else{
        /*
         * So la referencia es valida entonces debe bloquear el objeto
         * de ser posible
         */
        MinimalismBitVector memorylocation = _BitVector->get(index);
        if (!memorylocation.isOnUse()){
            calculateTime();
            pthread_mutex_unlock(&vHeap::mut);
            return false;
        }
        else{
            memorylocation.setOnUse(false);
            _BitVector->remove(index);
            _BitVector->add(memorylocation);
        }
    }
    calculateTime();
    pthread_mutex_unlock(&vHeap::mut);
    return true;
}





void vHeap::removeReference(vRef *pVRef){
    pthread_mutex_lock(&vHeap::mut);
    if(_vDebug)comienzo=time(NULL);
    int index = searchBitVector(pVRef->_Id);
    if (index == -1){
    }
    else{
        MinimalismBitVector memorylocation = _BitVector->get(index);
        if (memorylocation.getReferenceCounter() >0){
            memorylocation.removeReference();
            _BitVector->remove(index);
            _BitVector->add(memorylocation);
        }
    }
    calculateTime();
    pthread_mutex_unlock(&vHeap::mut);

}







void vHeap::collectGarbage()
{
    pthread_mutex_lock(&vHeap::mut);
    if(_vDebug)comienzo = time(NULL);
    _GarbageCollectorCalled++;
    MinimalismBitVector *memorylocation;
    for (int x = _BitVector->getLenght()-1; x > -1;x--){
        memorylocation = &_BitVector->getReference(x);
        if (memorylocation->getReferenceCounter() == 0){
            if (memorylocation->isChargedOnMemory()){
                memset(((ExplorationVHeap*)_Chunk + memorylocation->getOffset()),0,memorylocation->getWeight());
                _CurrentMemoryUsed -=memorylocation->getWeight();
            }
            else{
                char tmp[memorylocation->getWeight()];
                _HeapOfPage.getPage(memorylocation->getOffset() - _Weight,tmp);
                moveInPlaceOffset(x,memorylocation->getWeight());
            }
            _BitVector->remove(x);
            break;

        }
    }
    calculateTime();
    pthread_mutex_unlock(&vHeap::mut);
}






void vHeap::compact()
{
    pthread_mutex_lock(&vHeap::mut);
    if(_vDebug)comienzo=time(NULL);
    _MemoryCompactorCalled++;
    _MemoryBlocks = 0;
    if (_BitVector->getLenght() > 0){
        _MemoryBlocks = 1;
        MinimalismBitVector tmp1 = _BitVector->get(0);
        unsigned int space;
        MinimalismBitVector tmp2;
        MinimalismBitVector tmp3;
        for(int bitvectoractual = 1; bitvectoractual < _BitVector->getLenght();bitvectoractual++){
            tmp2=_BitVector->get(bitvectoractual);
            space= spaceBetween(tmp2,tmp1);
            if (space > 0){
                _MemoryBlocks++;
                for(int otherbitvector = bitvectoractual;otherbitvector <_BitVector->getLenght();otherbitvector++){
                    tmp3 = _BitVector->get(otherbitvector);
                    // && tmp3.getReferenceCounter() > 0  se quito esto en la verificacion
                    //comprobar que no resulte mal para el codigo
                    if(!tmp3.isOnUse() && tmp3.isChargedOnMemory()){
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
    calculateTime();
    pthread_mutex_unlock(&vHeap::mut);
}






void vHeap::print()
{
    pthread_mutex_lock(&vHeap::mut);
    MinimalismBitVector m;
    std::cout << "tamanio: " << _BitVector->getLenght() << " --- " << "memory on use: " << _CurrentMemoryUsed
              <<  std::endl <<  "==========================" << std::endl;
    for (int x = 0; x < _BitVector->getLenght(); x++){
        m = _BitVector->get(x);
        std::cout << "id: "<< m.getId() << " refcount: "<< m.getReferenceCounter() << " offset: " << m.getOffset() << " size: " << m.getWeight();
        if(m.isChargedOnMemory()){
        	std::cout << " on phisical memory" << std::endl;
        }
        else{
        	std::cout << " on disk memory" << std::endl;
        }

    }
    std::cout << "==========================" << std::endl;
    pthread_mutex_unlock(&vHeap::mut);
}




void vHeap::updateOnMemoryViewer()
{
    pthread_mutex_lock(&vHeap::mut);
    string message= "{\"open\":";
    string open = isRunning()?"true":"false";
    message.append(open);
    message.append(", \"memory-used\": ");
    message.append(intToString(_CurrentMemoryUsed));
    message.append(", \"memory-free\": ");
    message.append(intToString(int(int(_Weight + _OverWeight) -int(_CurrentMemoryUsed))));
    message.append(", \"paginate-object\": ");
    message.append(intToString(_PagedObject));
    message.append(", \"not-paginate-object\": ");
    message.append(intToString(_NotPagedObject));
    message.append(", \"free-called\": ");
    message.append(intToString(_vFreeCalled));
    message.append(", \"malloc-called\": ");
    message.append(intToString(_vMallocCalled));
    message.append(", \"garbage-collector-called\": ");
    message.append(intToString(_GarbageCollectorCalled));
    message.append(", \"compact-memory-called\": ");
    message.append(intToString(_MemoryCompactorCalled));
    message.append(", \"memory-blocks\": ");
    message.append(intToString(_MemoryBlocks));
    message.append("}");
    try{
        (*clientSocket) << message;
    }catch(SocketException &e){
        //std::cout << "no se pudo enviar el mensaje"<< std::endl;
        //std:cout << e.description() << std::endl;
    }

    pthread_mutex_unlock(&vHeap::mut);
}



/*
 *Fin de metodos de acceso al vHeap
 *============================================================
 */






int vHeap::searchBitVector(int pId)
{
    IIterator<MinimalismBitVector> *_iterator = _BitVector->getIterator();
    for(int x = 0; x < _BitVector->getLenght();x++){
        if (_iterator->getNext().getId() == pId){
            delete _iterator;
            return x;
        }
    }
    delete _iterator;
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






void vHeap::moveInPlaceOffset(int pIndex, int pWeight)
{
    int lenght = _BitVector->getLenght();
    if (pIndex < 0 || pIndex >= lenght)throw IndexOutBounds(pIndex);
    if (_BitVector->get(pIndex).getOffset() < _Weight)return;
    MinimalismBitVector * bitVector;
    for (; pIndex < lenght;pIndex++){
        bitVector = &_BitVector->getReference(pIndex);
        bitVector->setOffset(bitVector->getOffset() + pWeight);
    }

}





/*
 *
 * Metodos internos
 *
 *
 */

vRef vHeap::insertOnPhysicalMemory(unsigned int pSize)
{
    MinimalismBitVector toInsert;
    if (_BitVector->getLenght() > 0){
        IIterator<MinimalismBitVector> *iterator = _BitVector->getIterator();
        MinimalismBitVector tmp1 = iterator->getNext();
        int lenght = _BitVector->getLenght();
        int x = 1;
        if (tmp1.getOffset() > pSize){
            tmp1.setWeight(0);
            tmp1.setOffset(0);
        }
        else if (lenght > 1){
            for(; x < lenght;x++){
                if (spaceBetween(iterator->getCurrent(),tmp1) < pSize)
                    tmp1 = iterator->getNext();
                else break;
            }
            if(x == lenght && pSize > _Weight - tmp1.getOffset() -tmp1.getWeight()){
                _NotPagedObject--;
                calculateTime();
                pthread_mutex_unlock(&vHeap::mut);
                throw FullMemoryHeapException();
            }
        }
        toInsert = MinimalismBitVector(tmp1.getOffset() + tmp1.getWeight(),pSize);
        delete iterator;
    }
    else toInsert = MinimalismBitVector(0,pSize);
    toInsert.setReferenceCounter(2);
    toInsert.setOnUse(false);
    toInsert.setOnMemoryLocation(true);
    memset(((ExplorationVHeap*)_Chunk + toInsert.getOffset()),0,toInsert.getWeight());
    _CurrentMemoryUsed += pSize;
    _BitVector->add(toInsert);
    calculateTime();
    pthread_mutex_unlock(&vHeap::mut);
    return vRef(toInsert.getId(),true);
}





vRef vHeap::insertOnDiskMemory(unsigned int pSize)
{
    int pIndex = getIndexToPagine(pSize);
    MinimalismBitVector toEvaluate;
    if (pIndex != -1){
        toEvaluate = _BitVector->get(pIndex);
        // se esta tratando mal el tamanio del dato
        vObject * toReturn = (vObject*)((ExplorationVHeap*)_Chunk + toEvaluate.getOffset());
        unsigned int weight = toEvaluate.getWeight();
        int sizeOfDiskMemory = _HeapOfPage.getMemoryUsed();
        //if wight > fheap weight
        if (weight + _HeapOfPage.getMemoryUsed() > _OverWeight){
            _PagedObject--;
            calculateTime();
            pthread_mutex_unlock(&vHeap::mut);
            throw FullMemoryHeapException();
        }
        char  toWrite [weight];
        for (unsigned int x = 0; x < weight;x++){
            toWrite[x] = *((char*) (&toReturn) + x);
        }
        memset(((ExplorationVHeap*)_Chunk + toEvaluate.getOffset()),0,toEvaluate.getWeight());
        _HeapOfPage.paginate(toWrite,weight);

        _CurrentMemoryUsed += pSize;
        _BitVector->remove(pIndex);
        MinimalismBitVector toAdd(toEvaluate.getOffset(),pSize);
        toAdd.setReferenceCounter(2);
        toAdd.setOnUse(false);
        toAdd.setOnMemoryLocation(true);
        _BitVector->add(toAdd);
        toEvaluate.setOffset(_Weight + sizeOfDiskMemory);// - pSize);
        toEvaluate.setOnMemoryLocation(false);
        _BitVector->add(toEvaluate);
        toEvaluate = _BitVector->get(pIndex);
    }
    else{
        _NotPagedObject--;
        _PagedObject--;
        calculateTime();
        pthread_mutex_unlock(&vHeap::mut);
        throw FullMemoryHeapException();
    }

    calculateTime();
    pthread_mutex_unlock(&vHeap::mut);
    return vRef(toEvaluate.getId(),true);
}



int vHeap::getIndexToPagine(unsigned int pSize)
{
    MinimalismBitVector toEvaluate;
    for (int pIndex = 0;pIndex < _BitVector->getLenght(); pIndex++){
        toEvaluate = _BitVector->get(pIndex);
        if (toEvaluate.isChargedOnMemory()){
            if(toEvaluate.getWeight() >= pSize){
                if (!toEvaluate.isOnUse()){
                    return pIndex;
                }
            }
        }
        else break;
    }
    return -1;

}

int vHeap::getIndexOfPaginateData()
{
    for (int pIndex = 0;pIndex < _BitVector->getLenght(); pIndex++){
        if (!_BitVector->get(pIndex).isChargedOnMemory()){
            return pIndex;
        }
    }
    return -1;
}


/*
 *
 * Fin de Metodos internos
 *
 *
 */




