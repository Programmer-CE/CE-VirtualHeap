#include "lanconection/socket.h"

#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

class ServerSocket : private Socket
{
public:

    ServerSocket ( int port );
    ServerSocket (){}
    virtual ~ServerSocket();

    const ServerSocket& operator << ( const std::string& ) const;
    const ServerSocket& operator >> ( std::string& ) const;

    void accept ( ServerSocket& );

};

#endif // SERVERSOCKET_H
