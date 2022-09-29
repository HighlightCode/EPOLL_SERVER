#ifndef EPOLL_SERVICE_H
#define EPOLL_SERVICE_H

#include <stdio.h>
#include <map>
#include <sys/epoll.h>
#include "types.h"


class Session;

class EpollService
{
public:
    EpollService() : mEpollFd(-1), mListenSocket(-1), bIsServerRun(true) {}

    bool        Initialize(uint16 Port, uint32 MaxClient);

    Session*    CreateSession(SOCKET sock);

    void        DeleteSession(Session* _session);

    SOCKET      GetEPollFd() const { return mEpollFd; }

    void        EventLoop();

    bool        handleFd(struct epoll_event ev);

private:
    using ClientList = std::map<SOCKET, Session*>;

    bool        bIsServerRun;

    ClientList  mClientList;

    SOCKET      mEpollFd;

    SOCKET      mListenSocket;
};

extern EpollService* GEpollService;

#endif