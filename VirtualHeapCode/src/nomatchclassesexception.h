#include "vheapexception.h"
#ifndef NOMATCHCLASSESEXCEPTION_H
#define NOMATCHCLASSESEXCEPTION_H

class NoMatchClassesException: public VHeapException
{
public:
    const char* what(){
        return "Las clases de asignacion no son compatibles!";
    }
};

#endif // NOMATCHCLASSESEXCEPTION_H
