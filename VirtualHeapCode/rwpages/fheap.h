#ifndef FHEAP
#define FHEAP
#include <string>
#include "src/vobject.h"
#include "src/vref.h"
#include "list/List.h"
#include "list/Node.h"


class fHeap{
private:
    bool _isPage=false;
    int _memoryPage;
    int _totalData=0;
    int _totalDataMemory=0;
    char * _path;
    List<int> _weightList;
    void write(char * pToWrite, int pNumberOfCharacters);
    const char * read(int pFrom, int to);
public:
    /**
     * @brief fHeap, instancia un representador de memoria del disco
     * @param overWeight es el tamanio que tiene la memoria del disco
     * @param pPath es la direccion del archivo donde piensa escribirse la memoria disco
     */
    fHeap(int overWeight, char* pPath);
    /**
     * @brief paginate, metodo que se encarga de paginar una cadena de caracteres
     * @param pToWrite es la cadena de caracteres a paginar
     * @param pNumberOfCharacters es el numero de caracteres
     */
    void paginate(char * pToWrite,int pNumberOfCharacters);
    /**
     * @brief interchangePage, es el metodo de intercambio de dos objetos del mismo tamanio,
     * este metodo no esta sujeto si los dos datos tienen diferente tamanio, por
     * lo que es responsabilidad del programador utilizar este metodo con minuciosiodad
     * esto con el fin de aumentar la velocidad de esta operacion
     * @param pToWrite es el objeto a escribir
     * @param pOffset es el lugar de memoria en el cual se desea hacer el swap
     * @param toCopy es el string en el se escribe el dato leido
     * @return toCopy
     */
    const char * interchangePage(char * pToWrite,int pOffset, char * toCopy);

    /**
     * @brief getPage, obtiene un dato de la memoria de disco, lo elimina de la misma
     * pues si se usa este dato en memoria fisica se elimina de memoria de disco (memoria de paginacion)
     * @param pOffset es el corrimiento sobre memoria de disco (memoria de paginacion)
     * @param toCopy es el lugar donde se leera los datos
     * @return toCopy
     */
    const char * getPage(int pOffset, char * toCopy);

    int getMemoryUsed();

    /**
     * @brief deletePage, destruye el archivo abierto
     */
    void deletePage();
    ~fHeap();


};

#endif // FHEAP

