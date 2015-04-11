#include "vobject.h"

#ifndef VCHAR_H
#define VCHAR_H

class vChar:public vObject
{
    char _myChar;
public:
    vChar(char pChar);
    char getChar();
    void setChar(char pChar);
};

#endif // VCHAR_H
