#ifndef CONDITION_H
#define CONDITION_H

#include "MutexLock.h"
#include "noncopyable.h"
#include <pthread.h>

class Condition:noncopyable
{
public:
    explicit Condition(MutexLock &mutex):_mutex(mutex)
    {
        pthread_cond_init(&_cond, nullptr);
    }

    ~Condition()
    {
        pthread_cond_destroy(&_cond);
    }

    void wait()
    {
        pthread_cond_wait(&_cond,_mutex.getMutex());
    }

    void notify()
    {
        pthread_cond_signal(&_cond);
    }

    void notifyAll()
    {
        pthread_cond_broadcast(&_cond);
    }

private:
    MutexLock &_mutex;
    pthread_cond_t _cond;
};

#endif