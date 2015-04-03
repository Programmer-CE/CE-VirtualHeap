#include <cstdio>
#include <cstdlib>
#include <string>
#include <new>
#ifndef VOBJECT_H
#define VOBJECT_H

class vRef;

/**
 * @brief Es la clase que representa al objeto contenido en el vHeap
 * @see vHeap
 *
 */
class vObject
{
    vRef *pRef; /**< TODO */
public:
    /**
     * @brief es el constructor por defecto
     *
     */
    vObject();
    /**
     * @brief Retorna una cadena con el tipo de dato
     *
     * @return el tipo de dato
     */
    virtual const char *getType()const;
};

#endif // VOBJECT_H
