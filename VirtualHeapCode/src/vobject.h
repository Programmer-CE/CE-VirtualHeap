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
    virtual const char *getType()const;
};

#endif // VOBJECT_H
