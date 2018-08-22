/*
 *封装的时间单位，精确到微秒
 * 包括获取当前时间（调用gettimeofday），时间格式转换，
 * 时间大小的比较
 * */
#ifndef TIMEUNIT_H
#define TIMEUNIT_H

#include <string>
#include <stdint.h>  /* Definition of uint64_t */

class TimeUnit
{
public:
    TimeUnit();
    explicit TimeUnit(int64_t microSecondsSinceEpoch);

    void swap(TimeUnit& rhs)
    {
        std::swap(_microSecondsSinceEpoch,rhs._microSecondsSinceEpoch);
    }

    std::string ToString() const ;
    std::string ToFormatString() const;

    bool Valid() const
    {
        return _microSecondsSinceEpoch > 0;
    }

    int64_t microSecondsSinceEpoch() const
    {
        return _microSecondsSinceEpoch;
    }

    //获得当前时间
    static TimeUnit now();

    //获得一个非法的时间，初始值为0
    static TimeUnit invalid();

    static const int kMicroSecondsPerSecond = 1000 * 1000;

private:
    int64_t _microSecondsSinceEpoch;
};

inline bool operator<(const TimeUnit &lhs, const TimeUnit &rhs)
{
    return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator==(const TimeUnit &lhs, const TimeUnit &rhs)
{
    return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

inline bool operator<=(const TimeUnit &lhs, const TimeUnit &rhs)
{
    return lhs.microSecondsSinceEpoch() <= rhs.microSecondsSinceEpoch();
}

//将传入的时间参数转换为一个TimeUnit
inline TimeUnit addTime(TimeUnit timeUnit, double seconds)
{
    int64_t delta = static_cast<int64_t >(seconds*TimeUnit::kMicroSecondsPerSecond);
    return TimeUnit(timeUnit.microSecondsSinceEpoch() + delta);
}

inline double timeDifference(TimeUnit hi,TimeUnit lo)
{
    int64_t diff = hi.microSecondsSinceEpoch() - lo.microSecondsSinceEpoch();
    return static_cast<double >(diff);
}

#endif