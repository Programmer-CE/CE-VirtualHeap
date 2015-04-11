#include "vobject.h"

#ifndef VREF_H
#define VREF_H

/**
 * @brief La clase vRef
 *
 */
class vRef
{
    friend class vHeap;
    int _Id = -1; /**< TODO */
    /**
     * @brief Inicializa la vRef
     *
     * @param pId el identidicador del vRef
     * @param pDestroyRef la bandera que inidica si el vRef se
     * disminuye una referencia o no en el vHeap
     */
    vRef(int pId,bool pDestroyRef);


public:
    bool _fromVHeap; /**< TODO */
    bool _destroyRef; /**< TODO */
    /**
     * @brief Inicializa el vRef como un puntero nulo
     *
     */
    vRef(){}

    /**
     * @brief Es el constructor de copia, se encarga de agregar
     * una referencia si el vRef es un puntero nulo
     *
     * @param pRef es el dato a copiar
     */
    vRef(const vRef & pRef);

    /**
     * @brief Asignacion. Asigna una copia del dato a la referencia, si la
     * referencia es nula entoncen se lanza un error
     *
     * @param pVObject Es el dato a copiar en el vHeap
     * @return una referencia a un vRef
     * @throw NullPointerException si la referencia es nula
     */
    vRef &operator =(const vObject *pVObject);
    /**
     * @brief Es un metodo estatico que se encarga de reservar
     * memoria y asignar un dato a esa memoria
     *
     * @param pSize es el peso que se quiere reservar
     * @param pVObject es el dato que se quiere copia en el vHeap
     * @return vRef es la referencia al dato
     */
    static vRef assing(size_t pSize, const vObject *pVObject);



    static vRef reserve(size_t pSize);


    /**
     * @brief se asigna a una direccion de memoria, si la direccion
     * no pertenece al vHeap, es un puntero nulo
     *
     * @param pAddress es la direccion a referenciar
     * @return una referencia al vRef asignado
     */
    vRef &operator =(int pAddress);
    /**
     * @brief Operador de asignacion a un vRef se copian los datos
     * se disminuye en 1 la referencia actual y se aumenta en 1
     * la referencia a copiar
     *
     * @param pRef Es la referencia a referenciar
     * @return Una referencia a un vRef
     */
    vRef &operator =(vRef pRef);
    /**
     * @brief Operador de de-referencia, obtiene el dato que este apunta
     * alojado en memoria del vHeap. Si el puntero es invalido
     * arroja un NullPointerException como error
     * @throw NullPointerException si el puntero es invalido
     * @return retorna el dato alojado en el vHeap que este apunta
     */
    vObject *operator * ();
    /**
     * @brief Verifica si dos referencias (vRef) son iguales
     *
     * @param pVRef es el vRef con el cual se desea comparar
     * @return true si son iguales
     */
    bool operator ==(const vRef & pVRef);
    /**
     * @brief Obtiene el peso del objeto que apunta, arroja un error
     * si este es un puntero invalido
     * @throw NullPointerException si el puntero es invalido
     *
     * @return el peso del objeto que apunta
     */
    unsigned int getWeight();
    /**
     * @brief Obtiene el tipo de dato al cual la referencia (vRef) apunta, arroja un error
     * si este es un puntero invalido
     *
     * @throw NullPointerException si el puntero es invalido
     *
     * @return el tipo de dato del objeto al cual este apunta
     */
    std::string getType();
    /**
     * @brief Destruye el vRef y elimina una referencia si
     * la bandera de destruccion lo permite
     *
     */

    virtual ~vRef();

};

#endif // VREF_H
