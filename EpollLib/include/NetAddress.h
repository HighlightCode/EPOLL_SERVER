#ifndef NETADDRESS_H
#define NETADDRESS_H

#include "types.h"
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>

class NetAddress
{
public:
    NetAddress() = default;
    NetAddress(sockaddr_in _sockAddr);
    NetAddress(std::string ip, uint16 port);

    inline sockaddr_in&     GetSockAddr() {return mSockAddr;}
    inline uint16           GetPort() {return ::ntohs(mSockAddr.sin_port);}
    std::string             GetIpAddress();

public:
    in_addr                 Ip2Address(const char* ip);

private:
    sockaddr_in             mSockAddr = {};
};

#endif