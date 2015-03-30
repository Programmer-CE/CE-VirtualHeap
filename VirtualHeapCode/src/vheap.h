#include "ordinateList/doublelist.h"
#include "minimalismbitvector.h"
#include <string>
#include "vref.h"
#include "vobject.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "src/garbagecollectorthread.h"
#include <pthread.h>





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
    bool _isRunning;
    DoubleList<MinimalismBitVector> *_BitVector;
    GarbageCollectorThread *_GarbageCollector;
    int searchBitVector(int pId);
    vHeap()
    {
        mut = PTHREAD_MUTEX_INITIALIZER;
        _isRunning = true;
        _BitVector = new DoubleList<MinimalismBitVector>();
        _Chunk = malloc(_Weight);
        _CurrentMemoryUsed = 0;
        _OverWeight = 0;
        _GarbageCollector = new GarbageCollectorThread();
        _GarbageCollector->start();
    }
public:
    pthread_mutex_t mut;
    static vHeap* getInstance();
    vRef vMalloc(unsigned int pSize, string pType);
    void vFree(vRef* pRef);
    void set(vRef* pRef, const vObject *pObject);
    vObject* get(vRef * pRef);
    void get(vRef* pRef,const void * pAddress);
    /**
     * @brief get solicita una seccion de memoria la retorna si y solo si
     * esta direccion de memoria esta contemplada en la lista de bitvectors
     * @param pAddress
     * @return
     */
    vRef get(int pAddress);

    vRef addRef(const vRef* pRef);

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

    void *getDir(vRef * pVRef);

    void collectGarbage();

    bool isRunning(){
        return _isRunning;
    }
    void stop() {
        _isRunning = false;
    }

    void print();
    std::string getType(vRef * pRef);
    unsigned int getWight(vRef * pRef);

    virtual ~vHeap()
    {
        std::cout << "error" << std::endl;
        _GarbageCollector->stop();
        delete _GarbageCollector;
        delete _Instance;
        delete _BitVector;
        free(_Chunk);
    }
};

#endif // VHEAP_H
