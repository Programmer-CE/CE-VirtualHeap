#include "clientsocket.h"
#include "socketexception.h"


ClientSocket::ClientSocket ( std::string host, int port )
{
    if ( ! Socket::create() )
    {
        throw SocketException ( "No se puede crear el cliente." );
    }

    if ( ! Socket::connect ( host, port ) )
    {
        throw SocketException ( "No se puede enlazar al puerto." );
    }

}


const ClientSocket& ClientSocket::operator << ( const std::string& s ) const
{
    if (!Socket::sendMessage(s))
    {
        throw SocketException ( "No se puede leer al socket." );
    }

    return *this;

}


const ClientSocket& ClientSocket::operator >> ( std::string& s ) const
{
    if (!Socket::receiveMessage(s))
    {
        throw SocketException ( "No se pudo leer del socket." );
    }

    return *this;
}
