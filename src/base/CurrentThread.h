#ifndef CURRENTTHREAD_H
#define CURRENTTHREAD_H

#include <stdint.h>

namespace CurrentThread
{
    //应使用extern关键字，声明该变量，若没有该关键字，就会变成定义该变量，在头文件中定义变量极易引起变量的重定义错误
    extern __thread int t_cachedTid;//缓存当前线程标识符

    void cacheTid();
    inline int tid()
    {//返回当前线程的tid
        if (__builtin_expect(t_cachedTid == 0, 0))
        {
            cacheTid();
        }
        return t_cachedTid;
    }
}

#endif