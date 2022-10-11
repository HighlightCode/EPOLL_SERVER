#include <iostream>
#include <epollcxx.h>

#include "ServerSession.h"

int main()
{
    GEpollService = new EpollService;
    GThreadManager = new ThreadManager();
    GWorkerQueue = new WorkerQueue();
    ClientCallback *pClientCallback = new ClientCallback();

    GDatabaseJobManager = new DatabaseJobManager();

    if (false == DBConnector::Initialize("../../User.db"))
        return -1;

    if (false == GEpollService->Initialize(5000, 1000, pClientCallback))
        return -1;

    printf("[INFO] SERVER STATRTED \n");

    /*----------- LISTENING THREAD -----------*/
    GThreadManager->Launch([=]()
                           {
            while(true)
            {
                GEpollService->EventLoop();
            } });

    /*----------- DATABASE THREAD -----------*/
    GThreadManager->Launch([=]()
                           {
            while(true)
            {
                GDatabaseJobManager->ExecuteDatabaseJobs();
            } });

    /*----------- WORKER THREAD -----------*/
    for (int32 i = 0; i < 5; i++)
    {
        GThreadManager->Launch([=]()
                               {
            while(true)
            {
                GWorkerQueue->waitpop();
            } });
    }

    delete GThreadManager;
    delete GEpollService;
    delete pClientCallback;
}
