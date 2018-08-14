#include "EventLoopThreadPool.h"
#include "EventLoop.h"
#include "EventLoopThread.h"

#include <iostream>

EventLoopThreadPool::EventLoopThreadPool(EventLoop *mainLoop, int numThreads)
        : _mainLoop(mainLoop),
          _numThread(numThreads),
          _started(false),
          _next(0)
{
    if(numThreads <= 0)
    {
        std::cout<<"_numThreads <= 0"<<std::endl;
        abort();
    }
}

EventLoopThreadPool::~EventLoopThreadPool()
{

}

void EventLoopThreadPool::start()
{
    _mainLoop->assertInLoopThread();
    _started = true;
    for(int i=0; i < _numThread; i++)
    {
        std::shared_ptr<EventLoopThread> t(new EventLoopThread());
        _threads.push_back(t);
        _loops.push_back(t->startLoop());
    }
}

EventLoop* EventLoopThreadPool::getNextLoop()
{
    _mainLoop->assertInLoopThread();
    assert(_started);
    EventLoop* loop =_mainLoop;
    if(!_loops.empty())
    {
        loop = _loops[_next];
        _next = (_next +1) % _numThread;
    }
    return loop;
}