#include "vclassdetector.h"
#include <cstdlib>
#include <cstdio>
#include <typeinfo>

std::string vClassDetector::getClassOf(vObject *pObject)
{
    return std::string(typeid(*pObject).name());
}
