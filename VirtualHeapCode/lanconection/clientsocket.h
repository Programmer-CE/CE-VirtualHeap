#include "socket.h"
#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

class ClientSocket:private Socket
{
public:
    ClientSocket(std::string host, int port);
    virtual ~ClientSocket(){}
    const ClientSocket& operator << ( const std::string& ) const;
    const ClientSocket& operator >> ( std::string& ) const;
};

#endif // CLIENTSOCKET_H
