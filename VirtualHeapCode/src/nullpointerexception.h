#include "src/vheapexception.h"
#ifndef NULLPOINTEREXCEPTION_H
#define NULLPOINTEREXCEPTION_H

/**
 * @brief La clase NullPointerException, representa al error que
 * se envia cuando se accesa a una porcion de memoria a la cual no se
 * puede accesar
 */
class NullPointerException:public VHeapException
{
public:
    /**
     * @brief Es el metodo que contiene el mensaje de error
     * de la clase
     * @return una cadena de caracteres que contiene el error
     */
    const char* what(){
        return "Puntero nulo";
    }
};

#endif // NULLPOINTEREXCEPTION_H
