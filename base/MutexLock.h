#ifndef MUTEXLOCK_H
#define MUTEXLOCK_H

#include "noncopyable.h"
#include "CurrentThread.h"
#include <pthread.h>
#include <assert.h>


class MutexLock:noncopyable
{
public:
    MutexLock()
            :_owner(0)
    {
        pthread_mutex_init(&_mutex, nullptr);
    }

    ~MutexLock()
    {
        assert(_owner== 0);
        pthread_mutex_destroy(&_mutex);
    }

    bool isLockedByThisThread()
    {
        return _owner==CurrentThread::tid();
    }

    void assertLocked()
    {
        assert(isLockedByThisThread());
    }

    void lock()//仅供MutexLockGuard调用
    {
        pthread_mutex_lock(&_mutex);//先锁住
        _owner=CurrentThread::tid();//再改变_owner的值，即持有该锁的线程
    }

    void unlock()//仅供MutexLockGuard调用
    {
        _owner=0;//先改变_owner的值
        pthread_mutex_unlock(&_mutex);//再释放锁
    }

    pthread_mutex_t* getMutex()//仅供Condition调用
    {
        return &_mutex;
    }


private:
    pthread_mutex_t _mutex;
    pid_t _owner;
};

class MutexLockGuard:noncopyable{
public:
    explicit MutexLockGuard(MutexLock& mutex):_mutex(mutex)
    {
        _mutex.lock();
    }

    ~MutexLockGuard()
    {
        _mutex.unlock();
    }
private:
    MutexLock& _mutex;
};

//宏的作用，防止这种情况：MutexLockGuard(mutex),这样会产生一个临时变量，起不到锁住临界区的作用，正确写法MutexLockGuard lock(mutex)；
#define MutexLockGuard(x) stacic_assert(false,"missing MutexLockGuard var name")

#endif