#include "EventLoopThread.h"
#include "EventLoop.h"

#include <functional>

EventLoopThread::EventLoopThread()
        :_loop(nullptr),
         _exiting(false),
         _thread(std::bind(&EventLoopThread::threadFunc,this)),
         _cond(_mutex)
{

}

EventLoopThread::~EventLoopThread()
{
    _exiting=true;
    _loop->quit();
    _thread.join();
}

EventLoop* EventLoopThread::startLoop()
{
    assert(!_thread.started());
    _thread.start();

    //用条件变量等待线程启动完成
    {
        MutexLockGuard lock(_mutex);
        while(_loop== nullptr)
        {
            _cond.wait();
        }
    }

    return _loop;
}

void EventLoopThread::threadFunc()
{
    EventLoop loop;

    //用条件变量保护_loop，当其赋值好了之后通知startLoop()函数。
    {
        MutexLockGuard lock(_mutex);
        _loop=&loop;
        _cond.notify();
    }

    _loop->loop();
}

