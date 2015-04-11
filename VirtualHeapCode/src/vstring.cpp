#include "vstring.h"


vString::vString(string pString)
{
    _String = pString;
}



string vString::getString() const
{
    return _String;
}

void vString::setString(string String)
{
    _String = String;
}
