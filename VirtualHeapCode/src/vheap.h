#include "vref.h"
#include <string>
#include "ordinateList/doublelist.h"
#include "minimalismbitvector.h"
#include "vobject.h"
#include <cstdio>
#include <cstdlib>

using namespace std;

#ifndef VHEAP_H
#define VHEAP_H

class vHeap
{
    static vHeap *_Instance;
    static unsigned int _Weight;
    unsigned int _OverWeight;
    unsigned int _CurrentMemoryUsed;
    void *_Chunk;
    DoubleList<MinimalismBitVector> *_BitVector;
    int searchBitVector(int pId);
    vHeap()
    {
        _BitVector = new DoubleList<MinimalismBitVector>();
        _Chunk = malloc(_Weight);
        _CurrentMemoryUsed = 0;
        _OverWeight = 0;
    }
public:
    static vHeap* getInstance();
    vRef vMalloc(unsigned int pSize, string pType);
    void vFree(vRef* pRef);
    void set(vRef* pRef, vObject *pObject);
    vObject* get(vRef * pRef);
    /**
     * @brief get solicita una seccion de memoria la retorna si y solo si
     * esta direccion de memoria esta contemplada en la lista de bitvectors
     * @param pAddress
     * @return
     */
    vRef get(int pAddress);

    /**
     * @brief protect Protege una zona de memoria.
     * @param pVRef la referencia que pide la proteccion de la variable
     * @return true si la zona de memoria se pudo proteger, false si la zona de memoria
     * ya estaba protegida
     */
    bool protect(vRef * pVRef);
    /**
     * @brief removeReference Elimina la refencia hace que el conteo de referencias
     * hacia lo que apunta el vRef sea disminuido en 1
     * @param pVRef la referencia que quiere disminuir la referencia
     */
    void removeReference(vRef * pVRef);

    virtual ~vHeap()
    {
        delete _Instance;
        delete _BitVector;
        free(_Chunk);
    }
};

#endif // VHEAP_H
