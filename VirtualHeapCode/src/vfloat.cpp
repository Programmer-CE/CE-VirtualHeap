#include "vfloat.h"


vFloat::vFloat(float pFloat)
{
    _Float = pFloat;
}

float vFloat::getFloat() const
{
    return _Float;
}

void vFloat::setFloat(float pFloat)
{
    _Float = pFloat;
}
