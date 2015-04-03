#include "src/vheapexception.h"

#ifndef FULLMEMORYEXCEPTION_H
#define FULLMEMORYEXCEPTION_H

/**
 * @brief Es la clase que representa el error de memoria llena
 *
 */
class FullMemoryHeapException:public VHeapException{
public:
    /**
     * @brief Es el metodo que contiene el mensaje de error
     * de la clase
     * @return una cadena de caracteres que contiene el error
     */
    const char* what(){
        return "La memoria esta llena!";
    }
};
#endif // FULLMEMORYEXCEPTION_H
