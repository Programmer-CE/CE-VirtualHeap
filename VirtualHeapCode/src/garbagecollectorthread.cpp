#include "garbagecollectorthread.h"
#include <pthread.h>
#include "vheap.h"
#include <iostream>
#include "unistd.h"


GarbageCollectorThread::GarbageCollectorThread(unsigned int pTimeToWait)
{
    _TimeToWait = pTimeToWait;
}

void GarbageCollectorThread::internalRun()
{
    while(vHeap::getInstance()->isRunning()){
        vHeap::getInstance()->collectGarbage();
        usleep(_TimeToWait*1000);
    }
}
