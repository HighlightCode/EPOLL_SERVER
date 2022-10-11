#ifndef SESSION_H
#define SESSION_H

#include "types.h"
#include <map>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "circular_buffer.h"
#include "EPollInterface.h"
#include "DBJobContext.h"
#include "EpollService.h"
#include "NetAddress.h"

enum class ESessionState{
    cNone,
    cLogin,
    cChat
};

class Session
{
public:
    Session(SOCKET sock) : mConnected(false), mSocket(sock)
    { 
        mTcpCallback = nullptr;
        mService = nullptr;
        mState = ESessionState::cNone;
    }

    virtual ~Session();

    void OnConnect(sockaddr_in addr);
    void SetService(EpollService* pService) { mService = pService; }
    void DisConnect();
    //void OnRelease();
    void OnReceive();
    void OnSend(std::string SendPacket); // should be packet
    void BroadCast(std::string &pSendPacket){mService->BroadCast(pSendPacket);};

public:
    void SetNetAddress(NetAddress address) {mSockAddr = address;}
    inline SOCKET const GetSocket() {return mSocket;}
    inline bool         IsConnected() {return mConnected;}
    NetAddress          GetAddress() {return mSockAddr;}
    
    void                SetTcpSockCallback(ITcpSocketCallback* pTcpCallback) { mTcpCallback = pTcpCallback;}
    ITcpSocketCallback* GetTcpCallback() { return mTcpCallback;}

    void                DatabaseJobDone(DatabaseJobContext* result);
    bool                SendFlush(); 
    inline void         SetLogin() {mState = ESessionState::cLogin;}
    inline bool         IsLogon() {return mState== ESessionState::cLogin ? true : false;}
    inline void         SetChat() {mState = ESessionState::cChat;}
    inline bool         IsChat() {return mState== ESessionState::cChat ? true : false;}

protected:
    /* Contents Code Redef */
    virtual int32   OnRecv(BYTE* buffer, int32 len) {return len;}
    virtual void    OnSend(int32 len) {}   

private:
    bool            mConnected;
    ESessionState   mState;
    SOCKET          mSocket;
    EpollService*   mService;

    NetAddress      mSockAddr{};

    CircularBuffer  mSendBuffer{BUFSIZ};
    CircularBuffer  mReceiveBuffer{BUFSIZ};

    ITcpSocketCallback* mTcpCallback;
};


#endif