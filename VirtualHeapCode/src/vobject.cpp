#include "vobject.h"
#include "vheap.h"
#include "vref.h"
#include <typeinfo>
#include <iostream>
vObject::vObject()
{
}

const char *vObject::getType()const
{
    return typeid(*this).name();
}

