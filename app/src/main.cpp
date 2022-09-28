#include <iostream>
#include <epollcxx.h>

int main()
{
    NetAddress Net("127.0.0.1", 6000);

    std::cout << Net.GetIpAddress() << " " << Net.GetPort() << std::endl;
    
    GEpollService = new EpollService;
    GThreadManager = new ThreadManager();

    if(false == GEpollService->Initialize(5000, 1000))
        return -1;
    
    GThreadManager->Launch([=]()
    {
        while(true)
        {
            GEpollService->EventLoop();
        }
    });

    printf("[INFO] SERVER STATRTED \n");

    GThreadManager->Join();
    delete GEpollService;

}
