#ifndef EPOLL_SERVICE_H
#define EPOLL_SERVICE_H

#include <stdio.h>
#include <map>
#include "types.h"


class Session;

class EpollService
{
public:
    EpollService() : mEpollFd(-1), mListenSocket(-1) {}

    bool        Initialize(uint16 Port, uint32 MaxClient);

    Session*    CreateSession(SOCKET sock);

    void        DeleteSession(Session* _session);

    SOCKET      GetEPollFd() const { return mEpollFd; }

    void        EventLoop();

private:
    using ClientList = std::map<SOCKET, Session*>;

    ClientList  mClientList;

    SOCKET      mEpollFd;

    SOCKET      mListenSocket;
};

extern EpollService* GEpollService;

#endif