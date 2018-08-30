#include "TimeUnit.h"
#include <sys/time.h>
#include <stdio.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#undef __STDC_FORMAT_MACROS

TimeUnit::TimeUnit():_microSecondsSinceEpoch(0)
{

}

TimeUnit::TimeUnit(int64_t microSecondsSinceEpoch)
        :_microSecondsSinceEpoch(microSecondsSinceEpoch)
{

}

TimeUnit TimeUnit::now()
{
    /*
     * gettimeofday获取当前时间，将结果保存在timeval结构体中
     * timeval包含两个成员变量，秒和微秒
     * */
    struct timeval tv;
    gettimeofday(&tv, nullptr);

    int64_t seconds=tv.tv_sec;
    return TimeUnit(seconds*kMicroSecondsPerSecond+tv.tv_usec);
}

TimeUnit TimeUnit::invalid()
{
    return TimeUnit();
}

std::string TimeUnit::ToString() const
{
    char buf[32]={0};
    //秒
    int64_t seconds= static_cast<time_t >(_microSecondsSinceEpoch / kMicroSecondsPerSecond);
    //微秒
    int64_t microseconds= static_cast<time_t >(_microSecondsSinceEpoch % kMicroSecondsPerSecond);
    snprintf(buf, sizeof(buf)-1, "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
    return buf;
}

std::string TimeUnit::ToFormatString() const
{
    char buf[32]={0};
    time_t seconds= static_cast<time_t >(_microSecondsSinceEpoch / kMicroSecondsPerSecond);
    time_t microseconds= static_cast<time_t >(_microSecondsSinceEpoch % kMicroSecondsPerSecond);


    /*
     * time_t：日历时间，gmtime_r将日历时间转换为分解的时间，将结果保存在tm结构中，tm中最小的时间属性为second
     * */
    struct tm tm_time;
    gmtime_r(&seconds,&tm_time);

    snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
             tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
             tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
             microseconds);
    return buf;
}

