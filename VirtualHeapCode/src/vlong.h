#include "vobject.h"
#ifndef VLONG_H
#define VLONG_H

class vLong:public vObject
{
    long _Long;
public:
    vLong(long pLong);
    long getLong() const;
    void setLong(long pLong);
};

#endif // VLONG_H
