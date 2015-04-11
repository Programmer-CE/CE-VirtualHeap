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
    virtual bool isArray(){
        return false;
    }

public:
    /**
     * @brief es el constructor por defecto
     *
     */
    vObject();
    /**
     * @brief operator new, es el encargado de agregar el objeto a memoria del
     * vheap.
     * @param pSize es el tamanio del objeto
     * @param pRef es la referencia que apuntara al nuevo espacio de memoria
     * reservado
     * @return el puntero a memoria donde se colocara el objeto
     */
    void *operator new(size_t pSize, vRef * pRef);
    /**
     * @brief operator new[] Arroja un error, la declaracion de array en
     * el manejador de memoria vHeap no se realizara de esta manera
     * @param pSize el tamanio del objeto
     * @param pRef la referencia que apuntara al nuevo espacio de memoria
     * @return nada
     * @throw ArrayDeclarationIsInvalid
     */
    void *operator new[](size_t pSize, vRef * pRef);
    /**
     * @brief Retorna una cadena con el tipo de dato
     *
     * @return el tipo de dato
     */
    virtual const char *getType()const;
};

#endif // VOBJECT_H
