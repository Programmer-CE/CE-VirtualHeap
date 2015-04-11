#include "vobject.h"
#include "vheap.h"
#include "vref.h"
#include <typeinfo>
#include <iostream>
#include "arraydeclarationisinvalid.h"
#include "nullpointerexception.h"
vObject::vObject()
{
}

void* vObject::operator new(size_t pSize, vRef * pRef)
{
    if (!pRef)throw NullPointerException();
    *pRef = vRef::reserve(pSize);
    return (void*)**pRef;
}

void *vObject::operator new[](size_t pSize, vRef *pRef)
{
    throw ArrayDeclarationIsInvalid();
}

const char *vObject::getType()const
{
    return typeid(*this).name();
}
