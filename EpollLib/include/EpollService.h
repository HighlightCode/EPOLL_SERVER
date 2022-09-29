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
    EpollService() : bIsServerRun(true), mEpollFd(-1), mListenSocket(-1) {}

    bool        Initialize(uint16 Port, uint32 MaxClient);

    Session*    CreateSession(SOCKET _sock);

    void        DeleteSession(Session* _session);

    SOCKET      GetEPollFd() const { return mEpollFd; }

    void        EventLoop();

    void        ReleaseClient(Session* _session);

    bool        handleFd(struct epoll_event _ev);

private:
    using ClientList = std::map<SOCKET, Session*>;

    bool        bIsServerRun;

    SOCKET      mEpollFd;

    SOCKET      mListenSocket;

    ClientList  mClientList;
};

extern EpollService* GEpollService;

#endif