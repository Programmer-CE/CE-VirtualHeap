#include "serversocket.h"
#include "socketexception.h"


ServerSocket::ServerSocket(int port)
{
    if (!Socket::create())
    {
        throw SocketException ( "No se ha podido crear el servidor." );
    }

    if (!Socket::linkToPort(port))
    {
        throw SocketException ( "No se pudo enlazar con el puerto." );
    }

    if ( ! Socket::listen() )
    {
        throw SocketException ( "No se pudo escuchar al socket" );
    }

}

ServerSocket::~ServerSocket()
{
}


const ServerSocket& ServerSocket::operator << ( const std::string& s ) const
{
    if (!Socket::sendMessage(s))
    {
        throw SocketException ( "No se pudo escribir al socket." );
    }

    return *this;

}


const ServerSocket& ServerSocket::operator >> ( std::string& s ) const
{
    if (!Socket::receiveMessage(s))
    {
        throw SocketException ( "No se pudo leer del socket." );
    }

    return *this;
}

void ServerSocket::accept (ServerSocket& sock)
{
    if (!Socket::accept (sock))
    {
        throw SocketException ( "No se pudo aceptar al socket." );
    }
}
