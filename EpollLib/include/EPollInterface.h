#ifndef EPOLL_INTERFACE_H
#define EPOLL_INTERFACE_H

class Session;

class ITcpSocketCallback
{
public:
    ITcpSocketCallback(){}
    virtual ~ITcpSocketCallback(){}
    virtual bool OnConnect(bool bConnected) = 0;
    virtual int32 OnRecv(Session* pSession, unsigned int nRecvedLen,char* pBuffer) = 0;
    virtual bool OnSend(unsigned int nSentLen) = 0;
    virtual bool OnClose() = 0;
};

#endif