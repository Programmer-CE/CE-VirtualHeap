#include "ordinateList/doublelist.h"
#include "minimalismbitvector.h"
#include <string>
#include "vref.h"
#include "vobject.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "src/garbagecollectorthread.h"
#include <pthread.h>
#include "rwpages/fheap.h"
#include "lanconection/clientsocket.h"
#include "viewermemorythread.h"
#include "src/memorycompactor.h"
#include "lanconection/socketexception.h"



using namespace std;


#ifndef VHEAP_H
#define VHEAP_H


/**
 * @brief La clase vHeap, Representa a un manejador de memoria.
 * Cuenta con un compresor de memoria, garbage collector y
 * los metodos escenciales.
 */
class vHeap
{
    friend class vRef;
    static vHeap *_Instance;
    fHeap _HeapOfPage;
    static unsigned int _Weight;
    unsigned int _OverWeight;
    unsigned int _CurrentMemoryUsed;
    unsigned int _TimeToSleepThreads;
    void *_Chunk;
    bool _isRunning;
    int _vMallocCalled,_vFreeCalled, _GarbageCollectorCalled, _MemoryCompactorCalled;
    int _PagedObject, _NotPagedObject;
    int _MemoryBlocks;

    DoubleList<MinimalismBitVector> *_BitVector;
    GarbageCollectorThread *_GarbageCollector;
    memoryCompactor * _MemoryCompactor;
    ViewerMemoryThread *_ViewerUpdater;
    ClientSocket *clientSocket;
    /**
     * @brief searchBitVector
     * @param pId
     * @return
     */

    int searchBitVector(int pId);

    /**
     * @brief reallocMemory
     * @param pFrom
     * @param pAddressToAlloc
     * @return
     */
    MinimalismBitVector reallocMemory(const MinimalismBitVector *pFrom, unsigned int pAddressToAlloc);

    /**
     * @brief spaceBetween
     * @param pRefa
     * @param pRefb
     * @return
     */
    unsigned int spaceBetween(MinimalismBitVector pRefa,MinimalismBitVector pRefb);

    /**
     * @brief moveInPlaceOffset, mueve los datos despues del indice
     * indicado por el paramatro pIndex, mueve cada dato la cantidad
     * indicada por pWeight el otro parametro de esa funcion,
     * esta funcion es usada para la memoria alojada en el disco duro
     * por lo que el movimiento sera a nivel logico, no se realizara
     * ningun cambio en la memoria fisica
     * @param pIndex el indice desde el cual se revisara en la lista
     * de MinimalismBitVector
     * @param pWeight es la cantidad que se desea mover cada dato, si se
     * quiere mover hacia atras es negativo, si se quiere mover hacia
     * adelante debe ser positivo
     */
    void moveInPlaceOffset(int pIndex, int pWeight);



    /**
     * @brief insertOnPhysicalMemory
     * @param pSize
     */
    vRef insertOnPhysicalMemory(unsigned int pSize);

    /**
     * @brief insertOnDiskMemory
     * @param pSize
     */
    vRef insertOnDiskMemory(unsigned int pSize);


    int getIndexToPagine(unsigned int pSize);
    int getIndexOfPaginateData();


    /**
     * @brief Reserva memoria en el vHeap, ademas crea un objeto que
     * pertenece a la clase @link MinimalismBitVector
     * que representa al objeto en memoria, contando con un id, contador
     * de referencias, offset en memoria etc
     * @param pSize es el tamanio de memoria a reservar
     * @param pType es el tipo de objeto a reservar
     * @return un vRef que representa un puntero a la memoria reservada
     * por este puntero
     */
    vRef vMalloc(unsigned int pSize);



    /**
     * @brief vHeap Inicializa la el objeto vHeap
     */
    vHeap():_HeapOfPage(0,""), _vFreeCalled(0), _vMallocCalled(0),
        _GarbageCollectorCalled(0), _MemoryCompactorCalled(0),
        _PagedObject(0),_NotPagedObject(0)
    {
        _OverWeight = 400;
        _HeapOfPage = fHeap(_OverWeight, "pages.bin");
        _TimeToSleepThreads = 2000;
        _isRunning = true;
        _BitVector = new DoubleList<MinimalismBitVector>();
        _Chunk = malloc(_Weight);
        if (!_Chunk){
            std::cout << "error no hay suficient memoria" << std::endl;
            exit(0);
        }
        _CurrentMemoryUsed = 0;
        _GarbageCollector = new GarbageCollectorThread(_TimeToSleepThreads);
        _MemoryCompactor = new memoryCompactor(_TimeToSleepThreads);
        bool isConnected = true;
        try{
            clientSocket = new ClientSocket("localhost",30000);
        }
        catch(SocketException&){
            std::cout << "no se pudo conectar" << std::endl;
            isConnected=false;
        }
        if (isConnected){
            _ViewerUpdater = new ViewerMemoryThread();
            _ViewerUpdater->start();
        }
        _MemoryCompactor->start();
        _GarbageCollector->start();

    }
public:
    /**
     * @brief mut, Es el mutex de la clase, todos los metodos
     * de la clase estan protegidos por el mutex
     */
    static pthread_mutex_t mut;

    /**
     * @brief Obtiene la unica instancia, si no exite la crea
     * @return el puntero a la unica instancia
     */
    static vHeap* getInstance();
    /**
     * @brief Libera la memoria que un vRef apunta, si la referencia
     * no existe no hace nada
     * @param pRef es el puntero de un vRef a liberar de la memoria
     * que contiene el vHeap
     * @see vRef
     */
    void vFree(vRef* pRef);
    /**
     * @brief setea el contenido de un vRef
     * @param pRef la referencia a la cual se quiere setear un dato
     * @param pObject es el objeto que se sobrescribira en la zona de memoria
     * que representa el vRef
     * @throw NullPointerException si la referencia es nula o si esta no
     * existe
     * @throw NoMatchClassesException si la clase que el vref representa
     * es diferente de la clase a la cual el vObject pertenece
     */
    void set(vRef* pRef, const vObject *pObject);
    /**
     * @brief Obtiene el objeto contenido en el vRef, si el vRef
     * es invalido, o sea que ya se ha borrado o es una referencia nula
     * se lanza un error de NullPointerException
     * @param pRef el puntero de la vRef de la cual se quiere conseguir
     * el vObject
     * @throw NullPointerException si el vRef apunta a algun dato
     * no existente en el vHeap
     * @return el Objeto al cual pRef "apunta"
     */
    vObject* get(vRef * pRef);


    /**
     * @brief agrega una referencia a la referencia pasada por el parametro,
     * si la referencia pasada por el parametro es nula entonces no se hace nada
     * @param pRef el vRef al cual se desea agregar una referencia mas
     */
    void addRef(const vRef* pRef);

    /**
     * @brief protect Protege una zona de memoria.
     * @param pVRef la referencia que pide la proteccion de la variable
     * @return true si la zona de memoria se pudo proteger, false si la zona de memoria
     * ya estaba protegida
     */
    bool protect(vRef * pVRef);

    bool unprotect(vRef * pVRef);


    /**
     * @brief removeReference Elimina la refencia hace que el conteo de referencias
     * hacia lo que apunta el vRef sea disminuido en 1
     * @param pVRef la referencia que quiere disminuir la referencia
     */
    void removeReference(vRef * pVRef);

    /**
     * @brief es el metodo que lleva a cabo la accion que se le asigna
     * al @link GarbageCollectorThread
     */
    void collectGarbage();
    /**
     * @brief Es el metodo que se encarga de realizar las tareas asignadas
     * al compactador de memoria
     * @link memoryCompactor
     */
    void compact();
    /**
     * @brief verifica si el vHeap sigue corriendo
     * @return true, si el vHeap se mantiene activo
     */
    bool isRunning(){
        return _isRunning;
    }
    /**
     * @brief Imprime lo que se encuetra en el vHeap
     */
    void print();

    /**
     * @brief Obtiene un string con el tipo de dato de la referencia
     * @param pRef es la referencia de la cual se quiere solicitar el
     * tipo de dato
     * @return el tipo de dato que la referencia apunta
     */
    std::string getType(vRef * pRef);

    /**
     * @brief Obtiene un unsigned int que representa el peso de la referencia
     * @param pRef es la referencia de la cual se quiere solicitar el
     * peso del objeto
     * @return el peso del objeto que la referencia apunta
     */
    unsigned int getWight(vRef * pRef);



    /**
     * @brief updateOnMemoryViewer Manda las actualizaciones al memory viewer
     */
    void updateOnMemoryViewer();


    /**
     * @brief ~vHeap, detiene los threads, setea en false la bandera del
     * vHeap la cual dice si el vHeap esta corriendo,
     * Libera la memoria que ocupa.
     * Bloquea todo el proceso con mutex
     *
     */
    virtual ~vHeap()
    {
        pthread_mutex_lock(&vHeap::mut);
        std::cout<< "destroy vHeap called" << std::endl;
        _isRunning = false;
        pthread_mutex_unlock(&vHeap::mut);
        _GarbageCollector->stop();
        _MemoryCompactor->stop();
        pthread_mutex_lock(&vHeap::mut);
        std::cout << "error" << std::endl;
        delete _GarbageCollector;
        delete _MemoryCompactor;
        delete _Instance;
        delete _BitVector;
        free(_Chunk);
        pthread_mutex_unlock(&vHeap::mut);
    }
};

#endif // VHEAP_H
