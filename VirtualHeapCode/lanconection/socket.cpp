#include "socket.h"
#include "string.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "iostream"


const int Socket::MAXIMUN_LENGTH_TO_RECEIVE = 800;
const int Socket::MAXIMUN_HOST_NAME = 200;
const int Socket::MAXIMUN_CONNECTION = 5;


Socket::Socket(): _CrazySocket(-1)
{
    //llenamos de ceros la seccion de memoria en la que se encontrar el address
    memset(&_Address,0,sizeof(_Address));
}

bool Socket::create()
{
    // se crea el socket
    _CrazySocket = socket(AF_INET,SOCK_STREAM,0);
    //se verifica si es valido
    if (!isValid())return false;
    int on = 1;
    if (setsockopt(_CrazySocket,SOL_SOCKET,SO_REUSEADDR,(const char *)&on,sizeof(on)) == -1)return false;
    return true;
}

bool Socket::linkToPort(const int pPort)
{
    if(!isValid())return false;
    _Address.sin_family = AF_INET;
    _Address.sin_addr.s_addr = INADDR_ANY;
    _Address.sin_port = htons(pPort);
    int linkToReturn = ::bind(_CrazySocket,(struct sockaddr*)&_Address,sizeof(_Address));
    return linkToReturn != -1;
}

bool Socket::listen()
{
    if (!isValid())return false;

    int listenResults = ::listen(_CrazySocket,MAXIMUN_CONNECTION);
    return listenResults != -1;
}

bool Socket::accept(Socket &pOtherSocket)
{
    int addressLenght = sizeof(_Address);
    pOtherSocket._CrazySocket = ::accept(_CrazySocket,(sockaddr*)&_Address,(socklen_t *)&addressLenght);
    return pOtherSocket._CrazySocket > 0;
}

bool Socket::isValid()
{
    return _CrazySocket !=-1;
}

bool Socket::sendMessage(std::string message) const
{
    int sendStatus = send(_CrazySocket,message.c_str(),message.size(),MSG_NOSIGNAL);
    return sendStatus != -1;
}

int Socket::receiveMessage(std::string &message) const
{
    char messageReceive[MAXIMUN_LENGTH_TO_RECEIVE+1];
    message = "";
    memset(messageReceive,0,MAXIMUN_LENGTH_TO_RECEIVE+1);
    int receiveResults = ::recv(_CrazySocket,messageReceive,MAXIMUN_LENGTH_TO_RECEIVE,0);
    if (receiveResults == -1){
        std::cout << "error number: " << errno<< "in Socket receive" << std::endl;
        return 0;
    }
    else if (receiveResults == 0) {
        return 0;
    }
    else{
        message = messageReceive;
        return receiveResults;
    }

}



bool Socket::connect ( const std::string host, const int port )
{
  if (!isValid()) return false;

  _Address.sin_family = AF_INET;
  _Address.sin_port = htons (port);
  int status = inet_pton ( AF_INET, host.c_str(),&_Address.sin_addr);
  if ( errno == EAFNOSUPPORT ) return false;

  status = ::connect ( _CrazySocket, ( sockaddr * ) &_Address, sizeof ( _Address) );
  return status == 0;
}



void Socket::setNonBlocking ( const bool b )
{

  int opts;

  opts = fcntl ( _CrazySocket,
         F_GETFL );

  if ( opts < 0 )
    {
      return;
    }

  if ( b )
    opts = ( opts | O_NONBLOCK );
  else
    opts = ( opts & ~O_NONBLOCK );

  fcntl ( _CrazySocket,
      F_SETFL,opts );

}




Socket::~Socket()
{
    if(isValid()){::close(_CrazySocket);}
}
