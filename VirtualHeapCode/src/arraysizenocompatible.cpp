#include "arraysizenocompatible.h"

ArraySizeNoCompatible::ArraySizeNoCompatible()
{
}


const char *ArraySizeNoCompatible::what()
{
    return "El tamanio de los arreglos es diferente, por favor verifique su codigo";
}
