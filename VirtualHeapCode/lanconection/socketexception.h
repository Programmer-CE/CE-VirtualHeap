#ifndef SOCKETEXCEPTION_H
#define SOCKETEXCEPTION_H

#include <string>

class SocketException
{
    std::string _MessageWarning;
public:
    SocketException(std::string pMessageWarning):_MessageWarning(pMessageWarning){}
    ~SocketException(){}
    std::string MessageWarning() const;
    std::string description() { return _MessageWarning; }
};

#endif // SOCKETEXCEPTION_H
