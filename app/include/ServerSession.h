#include <iostream>
#include <string>
#include "../../EpollLib/include/EPollInterface.h"
#include "../../EpollLib/include/DBJobContext.h"
#include "../../EpollLib/include/Session.h"

class ClientCallback : public ITcpSocketCallback
{
public:
    ClientCallback(){}
    virtual ~ClientCallback() override {}

    virtual bool OnConnect(bool bConnected) override {
        std::cout << "Client Call back Initialized " << std::endl;
        return true;
    };

    virtual int32 OnRecv(Session* pSession, unsigned int nRecvedLen, char* pBuffer) override {

        std::string packet;
        for (int i = 0; i < nRecvedLen; i++)
        {
            char c = pBuffer[i];
            if (c >= 32 && c != 127)
            {
                packet += c;
            }

            else if (c == '\n' || c == '\r')
            {
                std::cout << packet << std::endl;
            }
        }
        if(packet == "new") 
        {
            pSession->SetLogin();
            const std::string sendPacket("\x1B[34mEnter Your Id \r\n\x1B[0m");
            pSession->OnSend(sendPacket);
        } 
        else if (pSession->IsLogon()) 
        { 
            FindPlayerDataContext* find = new FindPlayerDataContext(pSession->GetSocket());
            strncpy(&find->mPlayerName[0],packet.c_str(), packet.size());
            GDatabaseJobManager->PushDatabaseJobRequest(find);
            //const std::string sendPacket("\x1B[34mLogin Successed \r\n\x1B[0m");
            //pSession->SetChat();
            //pSession->OnSend(sendPacket);
        } 
        else if(pSession->IsChat()) 
        {
            std::string sendPacket("\x1B[34m");
            sendPacket += pSession->GetSocket();
            sendPacket += packet;
            sendPacket += "\r\n\x1B[0m";
            pSession->BroadCast(sendPacket);
        } else {
            const std::string sendPacket("\x1B[31mEnter Right Message \r\n\x1B[0m");
            pSession->OnSend(sendPacket);
        }
        return 0;
    };

    virtual bool OnSend(unsigned int nSentLen) override {
        return true;
    };

    virtual bool OnClose() override {
        return true;
    };
};