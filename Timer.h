#ifndef TIMER_H
#define TIMER_H

#include "base/noncopyable.h"
#include "base/TimeUnit.h"
#include "base/CallBack.h"
#include <functional>

class Timer:noncopyable{
public:

    Timer(const TimerCallBack tcb,TimeUnit when,double interval)
            :_timerCallBack(tcb),
             _expiration(when),
             _interval(interval),
             _repeat(interval>0)
    {

    }

    TimeUnit expiration() const
    {
        return _expiration;
    }

    bool repeat()const
    {
        return _repeat;
    }

    //重新设置timer的超时时间_expiration
    void restart(TimeUnit now)
    {
        if(_repeat)//若可以重复超时
        {
            _expiration=addTime(now,_interval);
        }
        else//否则，超时时间设置为一个非法值
        {
            _expiration=TimeUnit::invalid();
        }
    }

    void run()
    {
        _timerCallBack();
    }

private:
    const TimerCallBack _timerCallBack;//超时回调函数
    TimeUnit _expiration;//超时时刻
    const double _interval;//重复超时时的时间间隔
    const bool _repeat;//是否可以重复超时
};

#endif