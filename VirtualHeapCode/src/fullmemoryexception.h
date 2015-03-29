#include "src/vheapexception.h"

#ifndef FULLMEMORYEXCEPTION_H
#define FULLMEMORYEXCEPTION_H

class FullMemoryHeapException:public VHeapException{
public:
    const char* what(){
        return "La memoria esta llena!";
    }
};
#endif // FULLMEMORYEXCEPTION_H
