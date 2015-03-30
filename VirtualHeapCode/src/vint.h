#include "vobject.h"
#ifndef VINT_H
#define VINT_H

class vInt :public vObject
{
public:
    int _Int;
    vInt(int pInt);
};

#endif // VINT_H
