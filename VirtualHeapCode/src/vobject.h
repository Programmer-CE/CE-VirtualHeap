#include <cstdio>
#include <cstdlib>
#include <string>
#include <new>
#ifndef VOBJECT_H
#define VOBJECT_H

class vRef;
class vHeap;

/**
 * @brief Es la clase que representa al objeto contenido en el vHeap
 * @see vHeap
 *
 */
class vObject
{
    friend class vHeap;

    virtual bool isArray()const{
        return false;
    }

    /**
     * @brief getNumOfObjects en el caso de ser un vObject retorna 1
     * en el caso de ser un array, retorna el numero de objetos que
     * contiene
     * @return 1
     */
    virtual int getNumOfObjects() const{
        return 1;
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
