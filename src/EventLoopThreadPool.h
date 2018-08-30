#ifndef EVENTLOOPTHREADPOOL_H
#define EVENTLOOPTHREADPOOL_H

#include "base/noncopyable.h"
#include <memory>
#include <vector>

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : noncopyable
{
public:
    EventLoopThreadPool(EventLoop* mainLoop, int numThreads = 4);
    ~EventLoopThreadPool();

    void setThreadNum(int numThreads)
    {
        _numThread = numThreads;
    }

    EventLoop* getNextLoop();

    void start();

private:
    EventLoop* _mainLoop;
    bool _started;
    int _numThread;
    int _next;//只在loop中使用
    std::vector<std::shared_ptr<EventLoopThread> > _threads;
    std::vector<EventLoop*> _loops;
};

#endif