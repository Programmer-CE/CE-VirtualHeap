#include "arraydeclarationisinvalid.h"

ArrayDeclarationIsInvalid::ArrayDeclarationIsInvalid()
{
}

const char *ArrayDeclarationIsInvalid::what()
{
    return "no se puede instanciar un array de esta manera\nintentelo instanciado un vArray de esta misma biblioteca";
}
