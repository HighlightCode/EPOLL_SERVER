#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <errno.h>
#include <typeinfo>

#include "Session.h"
#include "NetAddress.h"
#include "../include/EpollService.h"


Session::~Session()
{
    close(mSocket);
    printf("[INFO] CLIENT SESSION DELETED \n");
}

void Session::OnConnect(sockaddr_in addr)
{
    mSockAddr = NetAddress(addr);

    // Set Socket To NonBlocking
    int flag = fcntl(mSocket, F_GETFL, 0);
    fcntl(mSocket, F_SETFL, flag | O_NONBLOCK);

    // Set off Nagle
    int opt = 1;
    setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(int));

    printf("[DEBUG] Client Connected: SOCKET_FD=%d, IP=%s, PORT=%d\n",mSocket, mSockAddr.GetIpAddress().c_str(), mSockAddr.GetPort()) ;

    mConnected = true;
}

void Session::DisConnect()
{
    if(!IsConnected()) return;

    // Linger Options
    linger lingerOption;
    lingerOption.l_onoff = 1;
	lingerOption.l_linger = 0;

    if (setsockopt(mSocket, SOL_SOCKET, SO_LINGER, (char*)&lingerOption, sizeof(linger)) < 0)
	{
		printf("[DEBUG] setsockopt linger option error . \n");
		return;
	}
    
    mConnected = false;
}

void Session::OnReceive()
{
    while(true)
    {
        int nread = read(mSocket, mReceiveBuffer.GetBuffer(), mReceiveBuffer.GetFreeSpaceSize());
        if (nread < 0)
        {
            if(errno == EAGAIN)
                break;
            DisConnect();
            break;
        } 
        else if(0 == nread)
        {
            printf("[INFO] Session Closed . Fd: %d \n", mSocket);
            DisConnect();
            break;
        }
        mReceiveBuffer.Commit(nread);
        printf("[DEBUG] Session Received %d Bytes . \n", nread);
    }

    if(!IsConnected())
        GEpollService->ReleaseClient(this);
}