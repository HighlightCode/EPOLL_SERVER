#ifndef WORKERQUEUE_H
#define WORKERQUEUE_H

#include <queue>
#include <iostream>
#include <functional>
#include <vector>
#include <mutex>
#include <sys/epoll.h>
#include "Lock.h"

class WorkerQueue
{
public:
    WorkerQueue();

    ~WorkerQueue();

    bool waitpop();
    
    bool pushSignal(struct epoll_event ev);

    //void Dispatch();

private:
    std::queue<struct epoll_event>  mWorkerQueue;

    Emutex_t        mLock;

    pthread_cond_t  mCond;

private:
    void Init();

    void Destroy();
};

extern WorkerQueue* GWorkerQueue;

#endif