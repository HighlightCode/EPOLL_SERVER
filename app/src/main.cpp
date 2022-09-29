#include <iostream>
#include <epollcxx.h>

int main()
{
    NetAddress Net("127.0.0.1", 6000);

    std::cout << Net.GetIpAddress() << " " << Net.GetPort() << std::endl;

    GEpollService = new EpollService;
    GThreadManager = new ThreadManager();
    GWorkerQueue = new WorkerQueue();

    if (false == GEpollService->Initialize(5000, 1000))
        return -1;

    GThreadManager->Launch([=]()
        {
            while(true)
            {
                GEpollService->EventLoop();
            } 
        }
    );

    for (int32 i = 0; i < 5; i++)
    {
        GThreadManager->Launch([=]()
        {
            while(true)
            {
                GWorkerQueue->waitpop();
            } 
        });
    }

    printf("[INFO] SERVER STATRTED \n");

    delete GThreadManager;
    delete GEpollService;
}
