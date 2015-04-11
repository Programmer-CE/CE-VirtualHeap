#include "vobject.h"

#ifndef VFLOAT_H
#define VFLOAT_H

class vFloat: public vObject
{
    float _Float;
public:
    vFloat(float pFloat);
    float getFloat() const;
    void setFloat(float pFloat);
};

#endif // VFLOAT_H
