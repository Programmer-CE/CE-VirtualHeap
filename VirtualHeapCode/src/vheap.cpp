#include "vheap.h"
#include "list/IIterator.h"
#include <cstring>
#include "fullmemoryexception.h"
#include "nullpointerexception.h"
#include "nomatchclassesexception.h"
#include "typeinfo"
#include "arraysizenocompatible.h"
#include "indexoutbounds.h"




// clase vacia que pesa 1 Byte
class ExplorationVHeap{

};


/*
 *Instancias Estasticas
 *============================================================
 */

vHeap* vHeap::_Instance = 0;

pthread_mutex_t vHeap::mut = PTHREAD_MUTEX_INITIALIZER;


//unsigned int vHeap::_Weight = 1024*1024*50;
unsigned int vHeap::_Weight = 400;

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
    if (_CurrentMemoryUsed + pSize < _Weight){
        // inserta en memoria fisica
        return insertOnPhysicalMemory(pSize);
    }
    else if (_CurrentMemoryUsed + pSize < _Weight + _OverWeight){
        // inserta en memoria fisica y baja un objeto a memoria de paginacion
        return insertOnDiskMemory(pSize);
    }
    else{
        //arroja error si la memoria esta llena del todo
        pthread_mutex_unlock(&vHeap::mut);
        throw FullMemoryHeapException();
    }
}





void vHeap::vFree(vRef *pRef)
{
    pthread_mutex_lock(&vHeap::mut);
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
        }
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
        //arroja un error en el caso de que la referencia no exista
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
        }
        else{
            toReturn= (vObject*)((ExplorationVHeap*)_Chunk + memorylocation->getOffset());
        }

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
    pthread_mutex_unlock(&vHeap::mut);
}







vObject* vHeap::get(vRef *pRef)
{
    /*
     * Recordar que aqui debe agregarse lo que respecta a paginación
     * se saca un objeto de igual peso y se intercambia de posicion
     * osea un swap en memoria, el otro caso y el peor es que no
     * se conseguir uno de igual peso y haya que realizar un corrimiento
     * por cada bitVector del fHeap. Si todos estan ocupados se agrega la solucion
     * arriba citada
     *
     */
    pthread_mutex_lock(&vHeap::mut);
    int index = searchBitVector(pRef->_Id);
    vObject * toReturn;
    if (index == -1){
        //se arroja un nullpointer exception si el objeto no existe
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
        }
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
    pthread_mutex_unlock(&vHeap::mut);
}






bool vHeap::protect(vRef *pVRef)
{
    pthread_mutex_lock(&vHeap::mut);
    int index = searchBitVector(pVRef->_Id);
    if (index == -1){
        /*
         * Si la referencia es invalida entonces se arroja un error
         */
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
            if (memorylocation->isChargedOnMemory()){
                memset(((ExplorationVHeap*)_Chunk + memorylocation->getOffset()),0,memorylocation->getWeight());
                _CurrentMemoryUsed -=memorylocation->getWeight();
            }
            else{
                char tmp[memorylocation->getWeight()];
                _HeapOfPage.getPage(memorylocation->getOffset() - _Weight,tmp);
            }
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
        std::cout << "id: "<< m.getId() << " refcount: "<< m.getReferenceCounter() << " offset: " << m.getOffset() << " size: " << m.getWeight()<< std::endl;
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
    vObject * toReturn = (vObject*)((ExplorationVHeap*)_Chunk + memorylocation.getOffset());
    return toReturn->getType();
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



/*
 *Fin de metodos de acceso al vHeap
 *============================================================
 */






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
        MinimalismBitVector tmp1 = _BitVector->get(0);
        if (tmp1.getOffset() > pSize){
            tmp1 = MinimalismBitVector(0,0);
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
        }

        toInsert = MinimalismBitVector(tmp1.getOffset() + tmp1.getWeight(),pSize);


    }
    else{
        toInsert = MinimalismBitVector(0,pSize);
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





vRef vHeap::insertOnDiskMemory(unsigned int pSize)
{
    int pIndex = getIndexToPagine(pSize);
    MinimalismBitVector toEvaluate;
    if (pIndex != -1){
        toEvaluate = _BitVector->get(pIndex);
        // se esta tratando mal el tamanio del dato
        vObject * toReturn = (vObject*)((ExplorationVHeap*)_Chunk + toEvaluate.getOffset());
        int weight = toEvaluate.getWeight();
        //if wight > fheap weight
        if (weight + _HeapOfPage.getMemoryUsed() > _OverWeight){
            pthread_mutex_unlock(&vHeap::mut);
            throw FullMemoryHeapException();
        }
        char  s [weight];
        for (int x = 0; x < weight;x++){
            s[x] = *((char*) (&toReturn) + x);
        }
        memset(((ExplorationVHeap*)_Chunk + toEvaluate.getOffset()),0,toEvaluate.getWeight());
        _HeapOfPage.paginate(s,pSize);

        _CurrentMemoryUsed += pSize;
        _BitVector->remove(pIndex);
        MinimalismBitVector toAdd(toEvaluate.getOffset(),pSize);
        toAdd.setReferenceCounter(2);
        toAdd.setOnUse(false);
        toAdd.setOnMemoryLocation(true);
        _BitVector->add(toAdd);
        toEvaluate.setOffset(_Weight + _HeapOfPage.getMemoryUsed() - pSize);
        toEvaluate.setOnMemoryLocation(false);
        _BitVector->add(toEvaluate);
        toEvaluate = _BitVector->get(pIndex);
    }
    else{
        pthread_mutex_unlock(&vHeap::mut);
        throw FullMemoryHeapException();
    }


    pthread_mutex_unlock(&vHeap::mut);
    return vRef(toEvaluate.getId(),true);
}



int vHeap::getIndexToPagine(unsigned int pSize)
{
    MinimalismBitVector toEvaluate;
    int bestIndexToInsertData = -1;
    int bestReferenceCounter = 0;
    bool bestIndexIsFoundIt = false;
    int tmp;
    for (int x = 0; x < _BitVector->getLenght(); x++){
        tmp = _BitVector->get(x).getReferenceCounter();
        if (tmp > 0 && bestReferenceCounter < tmp){
            bestReferenceCounter = tmp;
            bestIndexToInsertData = x;
        }
    }
    for (int pIndex = 0;pIndex < _BitVector->getLenght(); pIndex++){
        toEvaluate = _BitVector->get(pIndex);
        if (toEvaluate.isChargedOnMemory()){
            if(toEvaluate.getWeight() >= pSize){
                if (_BitVector->get(bestIndexToInsertData).getReferenceCounter()
                        >= toEvaluate.getReferenceCounter() && toEvaluate.getReferenceCounter() > 0){
                    bestIndexToInsertData = pIndex;
                    bestReferenceCounter = toEvaluate.getReferenceCounter();
                    bestIndexIsFoundIt = true;
                }
            }
        }
        else break;
    }
    if (bestIndexIsFoundIt){
        return bestIndexToInsertData;
    }
    else return -1;

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




