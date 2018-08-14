/*
 * EventLoop的功能是一个事件循环，它保证每个线程中只有一个事件循环。
 * */

#ifndef EVENTLOOP_H
#define EVENTLOOP_H



#include "base/noncopyable.h"
#include "base/CurrentThread.h"
#include "base/CallBack.h"
#include "base/TimeUnit.h"
#include "TimerManager.h"
#include "Timer.h"
#include "base/MutexLock.h"


#include <assert.h>
#include <unistd.h>
#include <memory>
#include <vector>
#include <iostream>

class Poller;
class TimerManager;
class TimeUnit;
class TimerQueue;
class Channel;

class EventLoop:noncopyable{
public:
    typedef std::vector<Channel*> ChannelList;
    typedef std::function<void()> Functor;

    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    //是否在本线程内
    bool isInLoopThread() const
    {
        return CurrentThread::tid()==_tid;
    }

    void assertInLoopThread()
    {
        //std::cout<<"CurrentThread::tid():"<<CurrentThread::tid()<<std::endl;
        //std::cout<<"_tid:"<<_tid<<std::endl;
        assert(isInLoopThread());
    }

    void updateChannel(Channel *channel);

    void wakeUp();//唤醒所在的IO线程

    EventLoop* getCurrentThreadEventLoop();

    //在该时刻执行
    TimerManager runAt(const TimeUnit& timeUnit,const TimerCallBack& tcb);

    //延迟一段时间执行,delay为延迟时间，单位为秒
    TimerManager runAfter(double delay,const TimerCallBack& tcb, const TimeUnit& timeUnit = TimeUnit::now());

    //每隔一段时间执行
    TimerManager runEvery(double interval,const TimerCallBack& tcb,  const TimeUnit& timeUnit = TimeUnit::now());

    //在当前Eventloop的IO线程中执行回调任务cb,这样可以安全地调用该线程中的回调函数，而不用加锁
    void runInLoopThread(const Functor &cb);

    //将cb放入任务队列中
    void queueInLoop(const Functor &cb);

    //移除Channel
    void removeChannel(Channel* channel);

private:
    void handRead();//唤醒作用
    void doPendingFuncs();//处理回调任务队列中的回调函数

    const pid_t _tid;//EventLoop所属的线程id
    bool _looping;//用来记录当前EventLoop是否在循环
    bool _quit;//是否准备退出循环


    std::unique_ptr<Poller> _poller;//每个Eventloop只有唯一的一个Poller，Eventloop间接拥有Poller（这里是以智能指针的形式）
    ChannelList _activeChannels;//该Eventloop中准备好的Channel
    TimeUnit _pollerReturnTime;//记录_poller的返回时刻
    std::unique_ptr<TimerQueue> _timerQueue;//timer队列

    int _wakeUpFd;//唤醒fd
    std::unique_ptr<Channel> _wakeUpChannel;//用来唤醒当前线程的Channel

    bool _isCallPendingFunc;//是否正在调用doPendingFuncs
    MutexLock _mutex;
    std::vector<Functor> _pendingFuncs;
};

#endif