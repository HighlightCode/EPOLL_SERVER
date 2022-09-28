#include "../include/NetAddress.h"
#include <stdio.h>
#include <memory.h>

NetAddress::NetAddress(sockaddr_in _sockAddr) : mSockAddr(_sockAddr)
{
}



NetAddress::NetAddress(std::string ip, uint16 port)
{
    ::memset(&mSockAddr, 0, sizeof(sockaddr_in));
    mSockAddr.sin_family = AF_INET;
    mSockAddr.sin_addr = Ip2Address(ip.c_str());
    mSockAddr.sin_port = ::htons(port);   
}

std::string NetAddress::GetIpAddress()
{
	char buffer[100];
	::inet_ntop(AF_INET, &mSockAddr.sin_addr, buffer, 100);
	return std::string(buffer);
}

in_addr NetAddress::Ip2Address(const char* ip) 
{
    in_addr ipAddress;
    ::inet_pton(AF_INET, ip, &ipAddress);
    return ipAddress;
}