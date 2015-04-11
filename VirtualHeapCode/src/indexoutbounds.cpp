#include "indexoutbounds.h"
#include <stdio.h>
#include <string.h>

IndexOutBounds::IndexOutBounds(int pIndex)
{
    _Index = pIndex;
}

const char *IndexOutBounds::what()
{
    throw strcat("Fuera de rango",(char*)(_Index));
}
