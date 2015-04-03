#include "vheapexception.h"
#ifndef NOMATCHCLASSESEXCEPTION_H
#define NOMATCHCLASSESEXCEPTION_H

/**
 * @brief Es la clase que representa al error cuando dos objeto no
 * pertenecen a la misma clase
 *
 */
class NoMatchClassesException: public VHeapException
{
public:
    /**
     * @brief Es el metodo que contiene el mensaje de error
     * de la clase
     * @return una cadena de caracteres que contiene el error
     */
    const char* what(){
        return "Las clases de asignacion no son compatibles!";
    }
};

#endif // NOMATCHCLASSESEXCEPTION_H
