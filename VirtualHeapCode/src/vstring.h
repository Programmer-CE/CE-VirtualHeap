#include "vobject.h"
#include <string>
#ifndef VSTRING_H
#define VSTRING_H

using namespace std;

class vString:public vObject
{
    string _String;
public:
    vString(string pString);
    string getString() const;
    void setString(string String);
};

#endif // VSTRING_H
