#include "garbagecollectorthread.h"
#include <pthread.h>
#include "vheap.h"
#include <iostream>
#include "unistd.h"


GarbageCollectorThread::GarbageCollectorThread()
{
    exec = true;
}

void GarbageCollectorThread::halt()
{
    exec = false;
}

void GarbageCollectorThread::internalRun()
{
    while(vHeap::getInstance()->isRunning()){
        pthread_mutex_lock(&vHeap::getInstance()->mut);
        vHeap::getInstance();
        vHeap::getInstance()->collectGarbage();
        vHeap::getInstance()->print();
        pthread_mutex_unlock(&(vHeap::getInstance()->mut));
        //usleep(42*1000);
        //std::cout << "garbage collector" << std::endl;
    }
    halt();
}

GarbageCollectorThread::~GarbageCollectorThread()
{
    while(vHeap::getInstance()->isRunning()){
        vHeap::getInstance()->stop();
        std::cout << "deleting" << std::endl;
    }
    std::cout << "is stopped" << std::endl;
}
