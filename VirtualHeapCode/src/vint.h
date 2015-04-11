#include "vobject.h"
#ifndef VINT_H
#define VINT_H

/**
 * @brief es la clase que representa al int comun
 *
 */
class vInt :public vObject
{
    int _Int; /**< TODO */
public:
    vInt();

    /**
     * @brief Es el contructor de vInt
     *
     * @param pInt es el int a contener
     */
    vInt(int pInt);
    /**
     * @brief obtiene el int contenido
     *
     * @return el int contenido
     */
    int getInt(){
        return _Int;
    }
    /**
     * @brief setea el int contenido
     *
     * @param pInt es el dato nuevo que se quiere setear
     */
    void setInt(int pInt){
        _Int = pInt;
    }
};

#endif // VINT_H
