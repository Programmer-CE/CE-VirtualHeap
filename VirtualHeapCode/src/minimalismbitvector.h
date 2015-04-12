#include <string>
#include <iostream>
using namespace std;


#ifndef MINIMALISMBITVECTOR_H
#define MINIMALISMBITVECTOR_H

/**
 * @brief
 *
 */
class MinimalismBitVector
{
    int _Id; /**< TODO */
    unsigned int _Weight; /**< TODO */
    int _ReferenceCounter; /**< TODO */
    bool _InUseFlag, _OnMemoryFlag; /**< TODO */
    unsigned int _Offset;
    static int Serial;

public:
    MinimalismBitVector();
    MinimalismBitVector(const MinimalismBitVector &othervariable);

    /**
     * @brief Constructor de los objetos de la clase, inicializa las variables principales
     * tales variables son aquellas que describen las principales caracteristicas de los
     * objetos en memoria, tales caracteristicas son: el offset o direccion relativa,
     * esta se mide en bytes y el corriemiento de bytes se da de 1 en 1, el peso del objeto y
     * finalmente el tipo del objeto.
     *
     * @param pOffset es la ubicacion del objeto que ademas es el identificador unico
     * de cada una de las variables en memoria virtual, debido a que los objetos no pueden
     * compartir la misma direccion de memoria.
     * @param pWeight es el tamanio del objeto al cual representa el objeto instanciado
     * @param pType es el tipo del objeto al cual esta clase representa.
     */
    MinimalismBitVector(unsigned int pOffset, unsigned int pWeight);
    /**
     * @brief suma en uno al contador de referencia
     *
     */
    void addReference();
    /**
     * @brief resta en uno el contador de referencias
     *
     */
    void removeReference();
    /**
     * @brief Verifica si el objeto que se representa a travez de las instancias de
     * MinimalismBitVector estan cargadas en memoria
     *
     * @return true, si el objeto esta en memoria, false si el objeto esta en disco
     */
    bool isChargedOnMemory() const;
    /**
     * @brief Verifica si el objeto esta siendo usado por algun otro recurso.
     *
     * @return true si el objeto esta en uso, false si el objeto no esta en uso
     */
    bool isOnUse()const;
    /**
     * @brief setea la bandera que indica si el objeto se esta usando, true si se quiere
     * que el objeto este en uso, false si se quiere que el objeto ya no este en uso.
     *
     * @param pOnUse es la bandera entrante que seteara al valor interno sobre el uso
     * de algun objeto en memoria virtual.
     */
    void setOnUse(bool pOnUse);
    /**
     * @brief Setea si el dato esta en memoria, si se quiere que el dato este en memoria se
     * pasa el valor true por parametro en caso contrario se pasa un false
     *
     * @param pMemoryLocationFlag
     */
    void setOnMemoryLocation(bool pMemoryLocationFlag);


    /**
     * @brief obtiene el peso del objeto que representa
     * @return el peso del objeto al cual representa
     */
    unsigned int getWeight() const;
    /**
     * @brief setea el peso del objeto al cual repesenta
     * @param pWeight el peso nuevo a setear
     */
    void setWeight(unsigned int pWeight);
    /**
     * @brief obtiene el el id del objeto
     * @return el id del objeto
     */
    int getId() const;

    /**
     * @brief retorna el movimiento relativo en memoria desde el inicio
     * del vHeap
     * @return el "offset" del dato al cual este representa
     */
    unsigned int getOffset() const;
    /**
     * @brief setea el offset al cual representa este objeto
     * @param Offset el nuevo offset
     */
    void setOffset(unsigned int pOffset);


    /**
     * @brief Obtiene la cantidad de referencias al cual este objeto, representa
     * @return la cantidad de referencias al objeto que representa
     */
    int getReferenceCounter() const;
    /**
     * @brief Setea la cantidad de referencias al objeto
     * @param pReferenceCounter la cantidad de referencias
     */
    void setReferenceCounter(int pReferenceCounter);
    /**
     * @brief operator <, se compara con otro objeto a partir de su offset
     * @param otherBitVector el otro objeto a comparar
     * @return true, si el offset del objeto que llama el metodo es menor
     * que el parametro enviado
     */
    bool operator <(const MinimalismBitVector& otherBitVector);
    /**
     * @brief operator <, se compara con otro objeto a partir de su offset
     * @param otherBitVector el otro objeto a comparar
     * @return true, si el offset del objeto que llama el metodo es menor
     * que el del parametro enviado
     */
    bool operator >(const MinimalismBitVector& otherBitVector);
    /**
     * @brief operator >, se compara con otro objeto a partir de su offset
     * @param otherBitVector el otro objeto a comparar
     * @return true, si el offset del objeto que llama el metodo es mayor
     * que el del parametro enviado
     */
    bool operator !=(const MinimalismBitVector& otherBitVector);
    /**
     * @brief operator !=, se compara con otro objeto a partir de su offset
     * @param otherBitVector el otro objeto a comparar
     * @return true, si el offset del objeto que llama el metodo es diferente
     * que el del parametro enviado
     */
    friend ostream &operator <<(ostream& os, const MinimalismBitVector & e){
        os << e.getId() << ":" << e.getReferenceCounter();
        return os;
    }

    /**
     * @brief destructor o liberador de memoria
     *
     */
    virtual ~MinimalismBitVector();
};

#endif // MINIMALISMBITVECTOR_H
