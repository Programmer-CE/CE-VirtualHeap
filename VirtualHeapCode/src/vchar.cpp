#include "vchar.h"



vChar::vChar(char pChar)
{
    _myChar = pChar;
}

char vChar::getChar()
{
    return _myChar;
}

void vChar::setChar(char pChar)
{
    _myChar = pChar;
}
