#include "../include/ThreadManager.h"
#include "../include/types.h"
#include "../include/ThreadTLS.h"

ThreadManager::ThreadManager()
{
    InitTLS();
}


ThreadManager::~ThreadManager()
{
    Join();
}


void ThreadManager::Launch(std::function<void(void)> callback)
{

}

void ThreadManager::Join()
{
    for(auto& t: mThread) {
        if(t.joinable())
            t.join();
    }
    mThread.clear();
}

void ThreadManager::InitTLS()
{
    static std::atomic<uint32> SThreadID = 1;
    LThreadID = SThreadID.fetch_add(1);
}

void ThreadManager::DestroyTLS()
{

}