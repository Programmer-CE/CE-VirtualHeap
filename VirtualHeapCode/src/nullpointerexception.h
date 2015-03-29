#include "src/vheapexception.h"
#ifndef NULLPOINTEREXCEPTION_H
#define NULLPOINTEREXCEPTION_H

class NullPointerException:public VHeapException
{
public:
    const char* what(){
        return "Puntero nulo";
    }
};

#endif // NULLPOINTEREXCEPTION_H
