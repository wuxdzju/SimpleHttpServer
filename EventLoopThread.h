#ifndef EVENTLOOPTHREAD_H
#define EVENTLOOPTHREAD_H

#include "base/noncopyable.h"
#include "base/Thread.h"
#include "base/MutexLock.h"
#include "base/Condition.h"

class EventLoop;

class EventLoopThread:noncopyable
{
public:
    EventLoopThread();
    ~EventLoopThread();
    EventLoop* startLoop();

private:
    void threadFunc();

    EventLoop* _loop;
    bool _exiting;
    Thread _thread;
    MutexLock _mutex;
    Condition _cond;
};

#endif