#ifndef TIMERMANAGER_H
#define TIMERMANAGER_H

#include <memory>

class Timer;

class TimerManager
{
public:
    explicit TimerManager(std::shared_ptr<Timer> timer)
            :_timer(timer)
    {

    }
private:
    std::weak_ptr<Timer> _timer;
};

#endif