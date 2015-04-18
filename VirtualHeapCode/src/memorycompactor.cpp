#include "memorycompactor.h"
#include "vheap.h"
#include "unistd.h"

memoryCompactor::memoryCompactor(unsigned int pTimeToSleep)
{
    _TimeToSleep = pTimeToSleep;
}


void memoryCompactor::internalRun()
{
    while(vHeap::getInstance()->isRunning()){
        vHeap::getInstance()->compact();
        usleep(_TimeToSleep*1000);
    }

}
