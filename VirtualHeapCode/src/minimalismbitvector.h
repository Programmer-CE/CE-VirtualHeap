#include <string>

using namespace std;


#ifndef MINIMALISMBITVECTOR_H
#define MINIMALISMBITVECTOR_H

/**
 * @brief
 *
 */
class MinimalismBitVector
{
    int *_Offset; /**< TODO */
    int _Weight; /**< TODO */
    string _Type; /**< TODO */
    int _ReferenceCounter; /**< TODO */
    bool _InUseFlag, _OnMemoryFlag; /**< TODO */

public:
    MinimalismBitVector();

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
    MinimalismBitVector(int pOffset, int pWeight, string pType);
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
    bool isChargedOnMemory();
    /**
     * @brief Verifica si el objeto esta siendo usado por algun otro recurso.
     *
     * @return true si el objeto esta en uso, false si el objeto no esta en uso
     */
    bool isOnUse();
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


    int getWeight() const;
    void setWeight(int pWeight);
    string getType() const;
    void setType(const string &pType);
    int *getOffset() const;
    void setOffset(int pOffset);
    int ReferenceCounter() const;
    void setReferenceCounter(int ReferenceCounter);

    bool operator <(const MinimalismBitVector& otherBitVector);
    bool operator >(const MinimalismBitVector& otherBitVector);
    bool operator !=(const MinimalismBitVector& otherBitVector);
    /**
     * @brief destructor o liberador de memoria
     *
     */
    virtual ~MinimalismBitVector();

};

#endif // MINIMALISMBITVECTOR_H
