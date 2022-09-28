#include "../include/ThreadManager.h"
#include "../include/types.h"
#include "../include/ThreadTLS.h"

ThreadManager* GThreadManager = nullptr;

ThreadManager::ThreadManager()
{
    InitTLS();
}


ThreadManager::~ThreadManager()
{
}


void ThreadManager::Launch(std::function<void(void)> callback)
{
    std::lock_guard<std::mutex> lock(mLock);

	mThread.push_back(std::thread([=]()
		{
			InitTLS();
			callback();
			DestroyTLS();
		}));
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