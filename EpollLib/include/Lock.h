#ifndef LOCK_H
#define LOCK_H

#include "types.h"
#include <pthread.h>

#define Emutex_t                pthread_mutex_t
#define Emutex_init(pmutex)     pthread_mutex_init(pmutex, NULL)
#define Emutex_destroy          pthread_mutex_destroy
#define Emutex_lock             pthread_mutex_lock
#define Emutex_unlock           pthread_mutex_unlock


#define Espinlock_t             pthread_spinlock_t
#define Espinlock_init(pspin)   pthread_spin_init(pspin, PTHREAD_PROCESS_PRIVATE)
#define Espinlock_destroy       pthread_spin_destroy
#define Espinlock_lock          pthread_spin_lock
#define Espinlock_unlock        pthread_spin_unlock


#define Erwlock_t               pthread_rwlock_t
#define Erwlock_init(prwlock)   pthread_rwlock_init(prwlock, NULL)
#define Erwlock_destroy         pthread_rwlock_destroy
#define Erwlock_rdlock          pthread_rwlock_rdlock
#define Erwlock_rdunlock        pthread_rwlock_unlock
#define Erwlock_wrlock          pthread_rwlock_wrlock
#define Erwlock_wrunlock        pthread_rwlock_unlock


/*--------------
    MutexLock
---------------*/
class MutexLock {
public:
    MutexLock() { Emutex_init(&_mutex); }
    ~MutexLock() { Emutex_destroy(&_mutex); }

    void lock() { Emutex_lock(&_mutex); }
    void unlock() { Emutex_unlock(&_mutex); }
protected:
    Emutex_t _mutex;
};

/*--------------
    SpinLock
---------------*/
class SpinLock {
public:
    SpinLock() { Espinlock_init(&_spin); }
    ~SpinLock() { Espinlock_destroy(&_spin); }

    void lock() { Espinlock_lock(&_spin); }
    void unlock() { Espinlock_unlock(&_spin); }
protected:
    Espinlock_t _spin;
};

/*------------
    RWLock
-------------*/
class RWLock {
public:
    RWLock()    { Erwlock_init(&_rwlock); }
    ~RWLock()   { Erwlock_destroy(&_rwlock); }

    void rdlock()   { Erwlock_rdlock(&_rwlock); }
    void rdunlock() { Erwlock_rdunlock(&_rwlock); }

    void wrlock()   { Erwlock_wrlock(&_rwlock); }
    void wrunlock() { Erwlock_wrunlock(&_rwlock); }

    void lock()     { rdlock(); }
    void unlock()   { rdunlock(); }
protected:
    Erwlock_t   _rwlock;
};

/*----------------
    LockGuard
-----------------*/
template<class T>
class LockGuard {
public:
    LockGuard() = delete;
    LockGuard(T& t) : _lock(t) { _lock.lock(); }
    ~LockGuard() { _lock.unlock(); }
protected:
    T& _lock;
};

#endif