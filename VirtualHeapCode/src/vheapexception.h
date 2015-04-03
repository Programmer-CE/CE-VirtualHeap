#ifndef VHEAPEXCEPTION_H
#define VHEAPEXCEPTION_H

/**
 * @brief La clase VHeapException, Representa cualquier error generado
 * por el vHeap
 */
class VHeapException
{
public:
    /**
     * @brief Es el metodo que contiene el mensaje de error
     * de la clase
     * @return una cadena de caracteres que contiene el error
     */
    virtual const char* what(){
        return "Ha ocurrido un error en el vHeap";
    }
};

#endif // VHEAPEXCEPTION_H
