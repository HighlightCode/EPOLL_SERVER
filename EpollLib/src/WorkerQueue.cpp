#include <sys/epoll.h>

#include "../include/WorkerQueue.h"
#include "../include/EpollService.h"

WorkerQueue* GWorkerQueue = nullptr;

WorkerQueue::WorkerQueue()
{
    Init();
}

WorkerQueue::~WorkerQueue()
{
    Destroy();
}

void WorkerQueue::Init()
{
    pthread_cond_init(&mCond,NULL);
    Emutex_init(&mLock);
}

void WorkerQueue::Destroy()
{
    pthread_cond_destroy(&mCond);
    Emutex_destroy(&mLock);
}

bool WorkerQueue::waitpop()
{
    struct epoll_event ev;
    Emutex_lock(&mLock);
    {
        pthread_cond_wait(&mCond, &mLock);
		while (!mWorkerQueue.empty())
		{
			ev = mWorkerQueue.front();
			mWorkerQueue.pop();
			GEpollService->handleFd(ev);
		}
    }
    Emutex_unlock(&mLock);

    return true;
}

bool WorkerQueue::pushSignal(struct epoll_event ev)
{
    Emutex_lock(&mLock);
    {
        mWorkerQueue.push(ev);
        printf("[INFO] Push Data To WorkerThreadQueue \n");
        pthread_cond_signal(&mCond);
    }
    Emutex_unlock(&mLock);

    return true;
}