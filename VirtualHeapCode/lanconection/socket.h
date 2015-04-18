#include "sys/socket.h"
#include "sys/types.h"
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include "google/protobuf/message.h"

#ifndef SOCKET_H
#define SOCKET_H



class Socket
{
    int _CrazySocket;
    sockaddr_in _Address;
public:
    static const int MAXIMUN_LENGTH_TO_RECEIVE;
    static const int MAXIMUN_CONNECTION;
    static const int MAXIMUN_HOST_NAME;
    Socket();
    /**
     * @brief crea el socket de manera que inicializa sus valores
     * @return retorna si el socket fue creado exitosamente
     */
    bool create();

    /**
     * @brief linkToPort, Enlazarse a un puerto
     * @param pPort puerto de coneccion
     * @return true, si el enlace pudo realizarse con exito
     */
    bool linkToPort(const int pPort);

    /**
     * @brief listen, escucha algo mediante el socket
     * @return true, si escucha algo por medio del socket de conexion
     */
    bool listen();


    bool accept(Socket & pOtherSocket);

    /**
     * @brief Retorna si el socket es valido
     * @return
     */
    bool isValid();


    /**
     * @brief Envia un mensaje
     * @param message es el mensaje que se desea enviar
     * @return si el mensaje fue enviado
     */
    bool sendMessage(std::string message) const;
    /**
     * @brief receiveMessage
     * @param message
     * @return
     */
    int receiveMessage(std::string &message) const;


    /**
     * @brief connect
     * @param host
     * @param port
     * @return
     */
    bool connect ( const std::string host, const int port );


    /**
     * @brief setNonBlocking
     * @param b
     */
    void setNonBlocking ( const bool b );

    virtual ~Socket();
};

#endif // SOCKET_H
