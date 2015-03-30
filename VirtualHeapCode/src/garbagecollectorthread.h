#include "crazythreads/crazythread.h"
#ifndef GARBAGECOLLECTORTHREAD_H
#define GARBAGECOLLECTORTHREAD_H

class GarbageCollectorThread:public CrazyThread
{
    bool exec;
public:
    GarbageCollectorThread();
    void halt();
    void internalRun();
    virtual ~GarbageCollectorThread();
};

#endif // GARBAGECOLLECTORTHREAD_H
