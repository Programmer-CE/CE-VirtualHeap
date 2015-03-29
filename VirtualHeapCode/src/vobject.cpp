#include "vobject.h"
#include "vheap.h"
#include "vref.h"
#include "vclassdetector.h"
#include <typeinfo>
vObject::vObject()
{
}

const char *vObject::getType()
{
    return typeid(*this).name();
}

void *vObject::operator new(size_t pSize, string pType) throw()
{
    vRef t= (vHeap::getInstance())->vMalloc(pSize, pType);
}


void vObject::operator delete(void *ptoDelete)
{

}

