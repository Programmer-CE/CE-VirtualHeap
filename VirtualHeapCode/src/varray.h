#include "vobject.h"
#include "nullpointerexception.h"
#include "vref.h"
#include "indexoutbounds.h"
#ifndef VARRAY_H
#define VARRAY_H


template <class E, int plenght>
class vArray:public vObject
{
    bool isArray(){
        return false;
    }
public:
    vArray();
    /**
     * @brief operator new, es el encargado de agregar los objeto a memoria del
     * vheap.
     * @param pSize es el tamanio del tipo de objeto
     * @param pRef es la referencia que apuntara al nuevo espacio de memoria
     * reservado
     * @return el puntero a memoria donde se colocara el objeto
     */
    void *operator new(size_t pSize, vRef * pRef);
    E &operator [](int pIndex);

};

template <class E, int plenght>
vArray<E,plenght>::vArray()
{

}


template <class E, int plenght>
void *vArray<E,plenght>::operator new(size_t pSize, vRef *pRef)
{
    if (!pRef)throw NullPointerException();
    *pRef = vRef::reserve(sizeof (E)*plenght);
    return (void*)**pRef;
}

template <class E, int plenght>
E &vArray<E,plenght>::operator [](int pIndex)
{
    if (pIndex < 0 || pIndex >= plenght)throw IndexOutBounds(pIndex);
    E* toReturn = (E*)this;
    for (int x = 0;x < plenght;x++){
        toReturn++;
        if (x == pIndex)break;
    }
    return *toReturn;
}





#endif // VARRAY_H
