/*
 * TimerQueue的作用时管理所有的定时器Timer
 * TimerQueue需要高效地组织目前尚未到期的Timer
 * 能够快速地根据当前时间找到已经到期的Timer
 * 也要能够高效地添加和删除Timer
 * 这里的做法是将TimeUnit和Timer*组合成一个pair，保存在set中
 * */

#ifndef TIMEQUEUE_H
#define TIMEQUEUE_H

#include "base/noncopyable.h"
#include "base/CallBack.h"
#include "Channel.h"
#include "base/TimeUnit.h"
#include "TimerManager.h"
#include "Timer.h"


#include <utility>
#include <set>
#include <memory>
#include <vector>

//class Timer;
class EventLoop;


class TimerQueue:noncopyable
{
public:

    typedef std::pair<TimeUnit, std::shared_ptr<Timer> > Entry;
    //使用set的好处是，系统会为我们自动去重，使用set的insert函数，第一次插入时，会新建一项，
    // 后面再对同样的项进行插入动作时，并不会插入新的项，而是修改已有的项
    typedef std::set<Entry> TimerList;
    typedef std::set<std::shared_ptr<Timer> > ActiveTimerSet;
    
    TimerQueue(EventLoop *loop);
    ~TimerQueue(){}

    //该函数会经常被其他线程调用，因此必须保证线程安全
    //为了保证线程安全，将addTimer拆成两部分，对外的接口通过Eventloop中的runInLoop函数调用addTimerInLoop
    //addTimer是供Eventloop使用的，Eventloop会把它封装成更好用的函数
    TimerManager addTimer(const TimerCallBack& tcb,TimeUnit when, double interval);

    void cancelTimer(TimerManager timerManager);

private:


    //当超时时调用
    void handRead();

    //addTimer的安全版本，供addTimer调用
    void addTimerInLoop(std::shared_ptr<Timer> timer);

    void cancelTimerInLoop(TimerManager timerManager);

    //该函数从_timers中移除所有已到期的Timer，并通过vector返回它们
    //std::vector<Entry> getExpired(TimeUnit now);
    std::vector<Entry> getExpired(TimeUnit now);

    //将ptimer插入到_timers中，并且当该ptimer是第一个元素时，返回true，否则返回false
    bool insert(std::shared_ptr<Timer> ptimer);
    void reset(const std::vector<Entry>& expired,TimeUnit now);

    EventLoop* _loop;
    const int _timerfd;
    Channel _timerfdChannel;
    //TimerList根据expiration排序?
    TimerList _timers;

    bool _callingExpiredTimers;
    ActiveTimerSet _activeTimerSets;
    ActiveTimerSet _cancelTimersets;
};

#endif