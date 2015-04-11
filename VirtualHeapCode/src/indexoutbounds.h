#include "vheapexception.h"

#ifndef INDEXOUTBOUNDS_H
#define INDEXOUTBOUNDS_H

class IndexOutBounds:VHeapException
{
    int _Index;
public:
    IndexOutBounds(int pIndex);

    // VHeapException interface
public:
    const char *what();
};

#endif // INDEXOUTBOUNDS_H
