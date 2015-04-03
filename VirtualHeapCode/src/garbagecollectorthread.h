#include "crazythreads/crazythread.h"
#ifndef GARBAGECOLLECTORTHREAD_H
#define GARBAGECOLLECTORTHREAD_H

/**
 * @brief Es la clase que se encarga de borrar los elementos
 * no referenciados
 *
 */
class GarbageCollectorThread:public CrazyThread
{
    unsigned int _TimeToWait;
public:
    /**
     * @brief Incializa el valor de espera del thread
     * @param pTimeToWait Es la cantidad de tiempo que espera
     * el thread hasta despertarse de nuevo, la cantidad de tiempo
     * es en milisegundos
     */
    GarbageCollectorThread(unsigned int pTimeToWait);

    // CrazyThread interface
protected:
    void internalRun();
};

#endif // GARBAGECOLLECTORTHREAD_H
