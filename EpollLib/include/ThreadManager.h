#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <thread>
#include <functional>
#include <vector>
#include <mutex>

/*-------------------
*   Thread Manager
--------------------*/
class ThreadManager
{
public:
    ThreadManager();
    ~ThreadManager();

    void Launch(std::function<void(void)> callback);
    void Join();

    static void InitTLS();
    static void DestroyTLS();

private:
    std::mutex                   mLock;
    std::vector<std::thread>     mThread;
};

extern ThreadManager* GThreadManager;

#endif