#ifndef SESSION_H
#define SESSION_H

#include "types.h"
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "circular_buffer.h"
#include "EPollInterface.h"
#include "NetAddress.h"


class Session
{
public:
    Session(SOCKET sock) : mConnected(false), mSocket(sock), mSendBuffer(BUFSIZ), mReceiveBuffer(BUFSIZ)
    { 
        mTcpCallback = nullptr;
    }

    virtual ~Session();

    void OnConnect(sockaddr_in addr);
    void DisConnect();
    //void OnRelease();
    void OnReceive();

public:
    void SetNetAddress(NetAddress address) {mSockAddr = address;}
    inline SOCKET const GetSocket() {return mSocket;}
    inline bool         IsConnected() {return mConnected;}
    NetAddress          GetAddress() {return mSockAddr;}
    
    void                SetTcpSockCallback(ITcpSocketCallback* pTcpCallback) { mTcpCallback = pTcpCallback;}
    ITcpSocketCallback* GetTcpCallback() { return mTcpCallback;}
    
protected:
    /* Contents Code Redef */
    virtual int32   OnRecv(BYTE* buffer, int32 len) {return len;}
    virtual void    OnSend(int32 len) {}   

private:
    bool            mConnected;
    SOCKET          mSocket;

    NetAddress      mSockAddr{};

    CircularBuffer  mSendBuffer;
    CircularBuffer  mReceiveBuffer;

    ITcpSocketCallback* mTcpCallback;
};


#endif