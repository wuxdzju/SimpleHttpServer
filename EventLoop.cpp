#include "EventLoop.h"
#include <stdint.h>
#include <sys/poll.h>
#include "Poller.h"
#include "Channel.h"
#include "TimerQueue.h"
#include "base/TimeUnit.h"

#include <sys/eventfd.h>
#include <iostream>
#include <functional>
#include <signal.h>



class IgnoreSigPipe
{
public:
    IgnoreSigPipe()
    {
        ::signal(SIGPIPE,SIG_IGN);
    }
};

//定义一个全局对象，在程序开始之前就忽略SIGPIPE信号
//全部对象的初始化在main函数之前进行

IgnoreSigPipe initobj;

__thread EventLoop* t_loopInThisThread= nullptr;
const int kPollTimeMs=10000;

static int createEventFd()
{
    int efd=eventfd(0,EFD_CLOEXEC | EFD_NONBLOCK);
    if(efd<0)
    {
        perror("createEventFd():");
        _exit(1);
    }
    return efd;
}

EventLoop::EventLoop()
        :_looping(false),
         _tid(CurrentThread::tid()),
        _poller(new Poller(this)),
        _quit(false),
         _isCallPendingFunc(false),
         _timerQueue(new TimerQueue(this)),
         _wakeUpFd(createEventFd()),
         _wakeUpChannel(new Channel(this,_wakeUpFd))
{
    if(t_loopInThisThread)
    {
        //Todo
    }
    else
    {
        t_loopInThisThread=this;
    }
    _wakeUpChannel->setReadCallBack(std::bind(&EventLoop::handRead,this));
    _wakeUpChannel->enableReading();
}

EventLoop::~EventLoop()
{
    assert(!_looping);
    close(_wakeUpFd);
    t_loopInThisThread= nullptr;
}

void EventLoop::loop()
{
    assert(!_looping);
    assertInLoopThread();
    _looping= true;
    _quit= false;

    while(!_quit)
    {
        _activeChannels.clear();
       _pollerReturnTime= _poller->poll(kPollTimeMs,&_activeChannels);
        for(Channel* channel:_activeChannels)
        {
            channel->handEvent(_pollerReturnTime);
        }

        doPendingFuncs();
    }

    std::cout<<"EventLoop"<<this<<" stop looping"<<std::endl;

    _looping= false;
}

void EventLoop::quit()
{
    _quit= true;
    //在必要时唤醒IO线程，让其及时关闭
    if(!isInLoopThread())
    {
        wakeUp();
    }
}

void EventLoop::updateChannel(Channel *channel)
{
    assert(channel->ownerLoop()==this);
    assertInLoopThread();
    _poller->updateChannel(channel);
}

EventLoop* EventLoop::getCurrentThreadEventLoop()
{
    return t_loopInThisThread;
}

TimerManager EventLoop::runAt(const TimeUnit &timeUnit, const TimerCallBack &tcb)
{
    return _timerQueue->addTimer(tcb,timeUnit,0);
}

TimerManager EventLoop::runAfter(double delay, const TimerCallBack &tcb)
{
    TimeUnit time(addTime(TimeUnit::now(),delay));
    runAt(time,tcb);
}

TimerManager EventLoop::runEvery(double interval, const TimerCallBack &tcb)
{
    TimeUnit time(addTime(TimeUnit::now(),interval));
    return _timerQueue->addTimer(tcb,time,interval);
}

void EventLoop::runInLoopThread(const EventLoop::Functor &cb)
{
    if(isInLoopThread())//如果在当前IO线程中，则直接进行回调
    {
        cb();
    }
    else
    {
        //否则，将回调函数加入回调队列中
        queueInLoop(cb);
    }
}

void EventLoop::queueInLoop(const EventLoop::Functor &cb)
{
    {
        MutexLockGuard Lock(_mutex);
        _pendingFuncs.push_back(cb);
    }

    //若不在当前IO线程中，或者正在调用doPendingFunc函数,则唤醒
    if(!isInLoopThread() || _isCallPendingFunc)
    {
        wakeUp();
    }
}

void EventLoop::doPendingFuncs()
{
    std::vector<Functor> funcs;
    _isCallPendingFunc= true;

    {
        MutexLockGuard lock(_mutex);
        funcs.swap(_pendingFuncs);
    }

    for(size_t i=0;i<funcs.size();i++)
    {
        funcs[i]();
    }

    _isCallPendingFunc= false;
}

//向_wakeUpfd中写一个字节的数据u
void EventLoop::wakeUp()
{
    uint64_t one=1;
    ssize_t n=write(_wakeUpFd,&one, sizeof(one));
    if(n!= sizeof(one))
    {
        perror("EventLoop::wakeUp():");
    }
}

//从_wakeUpFd中读一个字节
void EventLoop::handRead()
{
    uint64_t one=1;
    ssize_t n=read(_wakeUpFd,&one, sizeof(one));
    if(n!= sizeof(one))
    {
        perror("EventLoop::handRead():");
    }
}

void EventLoop::removeChannel(Channel *channel)
{

}
