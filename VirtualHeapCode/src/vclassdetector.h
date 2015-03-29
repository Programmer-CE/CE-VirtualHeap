#include <string>
#include <src/vobject.h>
#ifndef VCLASSDETECTOR_H
#define VCLASSDETECTOR_H

class vClassDetector
{
public:
    vClassDetector();
    static std::string getClassOf(vObject *pObject);
};

#endif // VCLASSDETECTOR_H
