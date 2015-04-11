#include "vheapexception.h"
#ifndef ARRAYDECLARATIONISINVALID_H
#define ARRAYDECLARATIONISINVALID_H

class ArrayDeclarationIsInvalid:public VHeapException
{
public:
    ArrayDeclarationIsInvalid();

    // VHeapException interface
public:
    const char *what();
};

#endif // ARRAYDECLARATIONISINVALID_H
