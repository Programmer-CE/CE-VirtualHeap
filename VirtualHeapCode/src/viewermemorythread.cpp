#include "viewermemorythread.h"
#include "vheap.h"
#include "unistd.h"
ViewerMemoryThread::ViewerMemoryThread()
{
}


void ViewerMemoryThread::internalRun()
{
    while(vHeap::getInstance()->isRunning()){
        vHeap::getInstance()->updateOnMemoryViewer();
        usleep(200*1000);
    }
}
