#include "TimerQueue.h"
#include "Timer.h"
#include "EventLoop.h"

#include <string.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <assert.h>

//timerfd_*系列函数的封装
namespace timerdetail
{
    int creadteTimerfd()
    {
        int timerfd=timerfd_create(CLOCK_MONOTONIC,TFD_CLOEXEC | TFD_NONBLOCK);
        if(timerfd<0)
        {
            //todo 最好封装一些打印错误的函数
            perror("createtTimerfd:");
            _exit(1);
        }
        return timerfd;
    }

    //从现在到when还剩多少时间，并将结果转化为timespec类型
    struct timespec howMuchTimeFromNow(TimeUnit when)
    {
        int64_t microseconds=when.microSecondsSinceEpoch()-TimeUnit::now().microSecondsSinceEpoch();
        if(microseconds<100)
        {
            microseconds=100;
        }

        struct timespec ts;
        ts.tv_sec= static_cast<time_t >(microseconds/TimeUnit::kMicroSecondsPerSecond);
        ts.tv_nsec= static_cast<long>((microseconds%TimeUnit::kMicroSecondsPerSecond)*1000);

        return ts;
    }

    //设置timerfd上的超时时刻,相当于在timerfd上注册可读事件
    void resetTimerfd(int timerfd,TimeUnit expiration)
    {
        struct itimerspec new_value;
        bzero(&new_value, sizeof(new_value));
        new_value.it_value=howMuchTimeFromNow(expiration);
        int ret=timerfd_settime(timerfd,0,&new_value, nullptr);//flg为0表示使用相对时间
        if(ret<0)
        {
            perror("setTimerfd:");
        }
    }

    void readTimerfd(int timerfd,TimeUnit now)
    {
        int64_t buf;
        size_t n=read(timerfd,&buf,sizeof(buf));
        if(n!=sizeof(buf))
        {
            perror("readTimerfd():");
        }
    }
}

using namespace timerdetail;


/*
 * 给所有的超时处理事件分配一个线程，该线程也拥有唯一的一个事件循环Eventloop
 * 该线程也拥有唯一的一个timerfd，和唯一的一个Channel
 * 以及拥有一个用来管理所有定时器的队列TimerList
 * */
TimerQueue::TimerQueue(EventLoop *loop)
        :_loop(loop),
        _timerfd(creadteTimerfd()),
        _timerfdChannel(loop,_timerfd)
{
    //设置Channel的readBackCall，并且让该Channel处于enablereading状态
    _timerfdChannel.setReadCallBack(std::bind(&TimerQueue::handRead,this));
    _timerfdChannel.enableReading();
}

//提供给外面的接口（主要供Eventloop使用，Eventloop会将其封装成更好用的函数）
//该函数将传进来的初始化定时器的参数封装成一个Timer，并将其插入到_times队列中
//如果该timer是队首元素（即第一个超时的timer），还需设定该_timerfd上的超时时刻（当时间到达该时刻时，在该fd上可读事件就绪）
//该函数返回一个TimerManager类，该类是对Timer的简单管理
TimerManager TimerQueue::addTimer(const TimerCallBack &tcb, TimeUnit when, double interval)
{
    std::shared_ptr<Timer> timer(new Timer(tcb, when, interval));

    _loop->runInLoopThread(
            std::bind(&TimerQueue::addTimerInLoop, this, timer));

    return TimerManager(timer);
}

void TimerQueue::addTimerInLoop(std::shared_ptr<Timer> timer)
{
    _loop->assertInLoopThread();

    bool isFirstOne=insert(timer);

    //如果该timer是第一个超时的，将该定时器的的超时时间设定为_timerfd的超时时间
    if(isFirstOne)
    {
        resetTimerfd(_timerfd,timer->expiration());
    }
}

//将一个timer插入到TimerList中去，并判断此timer是否是第一个超时的timer
bool TimerQueue::insert(std::shared_ptr<Timer> ptimer)
{
    _loop->assertInLoopThread();
    assert(_timers.size() == _activeTimerSets.size());

    bool isFirstOne= false;
    TimeUnit when=ptimer->expiration();
    TimerList::iterator it=_timers.begin();
    if(it==_timers.end() || when < it->first)
    {
        isFirstOne=true;
    }

    {
        std::pair<TimerList::iterator,bool> res
                = _timers.insert( std::make_pair(when, ptimer) );
        assert(res.second);
    }

    {
        std::pair<ActiveTimerSet::iterator,bool> res
                = _activeTimerSets.insert(ptimer);
        assert(res.second);
    }

    assert(_timers.size() == _activeTimerSets.size());

    return isFirstOne;
}

//重新设置已经超时的定时器,并重新设置_timerfd的超时时刻(相当于注册_timerfd上的可读时间)
void TimerQueue::reset(const std::vector<Entry> &expired, TimeUnit now)
{
    TimeUnit nextEpiration;

    for(std::vector<Entry>::const_iterator it=expired.begin();it!=expired.end();++it)
    {
        if(it->second->repeat()
           && _cancelTimersets.find(it->second) == _cancelTimersets.end())
        {
            it->second->restart(now);//重新设置该timer的_expiration
            insert(it->second);
        }
    }

    if(!_timers.empty())
    {
        nextEpiration=_timers.begin()->second->expiration();
    }

    if(nextEpiration.Valid())
    {
        resetTimerfd(_timerfd,nextEpiration);
    }
}

//该函数从_timers中移除所有已到期的Timer，并通过vector返回它们
//通过该函数可以获得当前所有超时的timer
std::vector<TimerQueue::Entry> TimerQueue::getExpired(TimeUnit now)
{
    assert(_timers.size() == _activeTimerSets.size());

    std::vector<Entry> expired;
    Entry sentry=std::make_pair(now, std::shared_ptr<Timer>(reinterpret_cast<Timer*>(UINTPTR_MAX)));
    TimerList::iterator it=_timers.lower_bound(sentry);
    assert(it==_timers.end() || now<it->first);
    for(TimerList::iterator iter=_timers.begin(); iter != it; iter++)
    {
        expired.push_back(*iter);
    }

    //删除_timers中已过期的timer
    _timers.erase(_timers.begin(),it);

    for(Entry entry : expired)
    {
        size_t n = _activeTimerSets.erase(entry.second);
        assert(n == 1);
    }

    return expired;
}

void TimerQueue::handRead()
{
    _loop->assertInLoopThread();
    TimeUnit now(TimeUnit::now());

    //当_timerfd超时时，该文件描述符上的读事件就绪，readTimerfd对该读事件做出响应
    readTimerfd(_timerfd,now);


    std::vector<Entry> expired=getExpired(now);

    _callingExpiredTimers = true;
    _cancelTimersets.clear();
    //安全地调用外面的临界区
    for(std::vector<Entry>::iterator it=expired.begin();it!=expired.end();++it)
    {
        it->second->run();
    }
    _callingExpiredTimers = false;

    //重新设置expired事件，并且重新设置_timefd的超时时刻(重新注册可读事件)
    reset(expired,now);
}

void TimerQueue::cancelTimer(TimerManager timerManager)
{
    _loop->runInLoopThread
            (std::bind(&TimerQueue::cancelTimerInLoop, this, timerManager));
}

void TimerQueue::cancelTimerInLoop(TimerManager timerManager)
{
    _loop->assertInLoopThread();
    assert(_timers.size() == _activeTimerSets.size());
    std::shared_ptr<Timer> timer(timerManager.getTimer());
    ActiveTimerSet::iterator it = _activeTimerSets.find(timer);
    if(it != _activeTimerSets.end())
    {

        size_t  n = _timers.erase(Entry((*it)->expiration(),(*it)));
        assert(n == 1);
        _activeTimerSets.erase(*it);
    }
    else if (_callingExpiredTimers)
    {
        _cancelTimersets.insert(timer);
    }
    assert(_timers.size() == _activeTimerSets.size());
}



