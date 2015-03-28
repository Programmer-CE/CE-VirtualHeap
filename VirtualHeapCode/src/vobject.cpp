#include "vobject.h"
#include <typeinfo>
vObject::vObject()
{
}

const char *vObject::getType()
{
    return typeid(*this).name();
}
