#include "vheapexception.h"

#ifndef ARRAYSIZENOCOMPATIBLE_H
#define ARRAYSIZENOCOMPATIBLE_H

class ArraySizeNoCompatible:public VHeapException
{
public:
    ArraySizeNoCompatible();

    // VHeapException interface
    const char *what();
};

#endif // ARRAYSIZENOCOMPATIBLE_H
