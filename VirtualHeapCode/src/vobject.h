#include <cstdio>
#include <cstdlib>
#include <string>
#include <new>
#ifndef VOBJECT_H
#define VOBJECT_H

class vRef;

class vObject
{
    vRef *pRef;
public:
    vObject();
    virtual const char *getType();
    void *operator new (size_t pSize, std::string pType) throw();
    void operator delete ( void *ptoDelete);

};

#endif // VOBJECT_H
