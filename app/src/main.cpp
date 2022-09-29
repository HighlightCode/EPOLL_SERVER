#include <iostream>
#include <epollcxx.h>

int main()
{
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
