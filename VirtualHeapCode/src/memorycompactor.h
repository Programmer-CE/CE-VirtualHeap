#include "crazythreads/crazythread.h"
#ifndef MEMORYCOMPACTOR_H
#define MEMORYCOMPACTOR_H

/**
 * @brief Es la clase que se encarga de realizar la compactacion
 * de los datos en el vHeap
 *
 */
class memoryCompactor:public CrazyThread
{
    unsigned int _TimeToSleep;
public:
    /**
     * @brief Inicializa el objeto, y ademas el tiempo que el thread
     * va a dormir
     * @param pTimeToSleep es la cantidad de tiempo que dura el thread en dormir
     * el tiempo es en milisegundos
     */
    memoryCompactor(unsigned int pTimeToSleep);

    // CrazyThread interface
protected:
    /**
     * @brief
     *
     */
    void internalRun();
};



#endif // MEMORYCOMPACTOR_H
