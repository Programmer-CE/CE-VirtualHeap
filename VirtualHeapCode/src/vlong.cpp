#include "vlong.h"


vLong::vLong(long pLong)
{
    setLong(pLong);
}

long vLong::getLong() const
{
    return _Long;
}

void vLong::setLong(long pLong)
{
    _Long = pLong;
}
