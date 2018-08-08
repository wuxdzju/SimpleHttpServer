#ifndef TIMERMANAGER_H
#define TIMERMANAGER_H

class Timer;


class TimerManager
{
public:
    explicit TimerManager(Timer* timer)
            :_timer(timer)
    {

    }
private:
    Timer* _timer;
};

#endif