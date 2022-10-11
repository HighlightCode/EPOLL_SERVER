#include <iostream>
#include "../../EpollLib/include/EPollInterface.h"


class ClientCallback : public ITcpSocketCallback
{
public:
    ClientCallback(){}
    virtual ~ClientCallback() override {}

    virtual bool OnConnect(bool bConnected) override {
        std::cout << "Client Call back Initialized " << std::endl;
        return true;
    };

    virtual int32 OnRecv(unsigned int nRecvedLen, char* pBuffer) override {
        std::cout << "Client Call back Established " << std::endl;
        return 0;
    };

    virtual bool OnSend(unsigned int nSentLen) override {
        return true;
    };

    virtual bool OnClose() override {
        return true;
    };
};