#ifndef EPOLL_SERVICE_H
#define EPOLL_SERVICE_H

#include <stdio.h>
#include <map>
#include <string>
#include <sys/epoll.h>
#include "types.h"

class ITcpSocketCallback;

class Session;

class EpollService
{
public:
    EpollService() : bIsServerRun(true), mEpollFd(-1), mListenSocket(-1) {
        mTcpCallback = nullptr;
    }

    bool        Initialize(uint16 Port, uint32 MaxClient, ITcpSocketCallback* Callback = nullptr);

    Session*    CreateSession(SOCKET _sock);

    void        DeleteSession(Session* _session);

    SOCKET      GetEpollFd() const { return mEpollFd; }

    void        EventLoop();

    void        ReleaseClient(Session* _session);

    bool        handleFd(struct epoll_event _ev);

    inline void SetSockCallback(ITcpSocketCallback* pTcpCallback) { mTcpCallback = pTcpCallback;}
    
    void        FlushClientSend();

    void        BroadCast(std::string& pMessage);

    void        DispatchDatabaseJobResults() ;

private:
    using ClientList = std::map<SOCKET, Session*>;

    bool        bIsServerRun;

    SOCKET      mEpollFd;

    SOCKET      mListenSocket;

    ClientList  mClientList;

    ITcpSocketCallback* mTcpCallback;
};

extern EpollService* GEpollService;

#endif