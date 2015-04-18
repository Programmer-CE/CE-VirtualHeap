#include "../crazythreads/crazythread.h"

#ifndef VIEWERMEMORYTHREAD_H
#define VIEWERMEMORYTHREAD_H

class ViewerMemoryThread: public CrazyThread
{
public:
    ViewerMemoryThread();

    // CrazyThread interface
protected:
    void internalRun();
};

#endif // VIEWERMEMORYTHREAD_H
