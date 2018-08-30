/*
 * 主要封装了线程的创建和等待结束(join)
 * 为保持一致性，所有的线程创建都应通过此类完成
 * */

#ifndef THREAD_H
#define THREAD_H



#include <pthread.h>
#include <functional>
#include "noncopyable.h"
#include <memory>

class Thread:noncopyable
{
public:
    typedef std::function<void()> ThreadFunc;
    explicit Thread(const ThreadFunc&);
    ~Thread();

    void start();
    int join();
    bool started() const{return _started;}
    pid_t tid() const {return *_tid;}

private:
    bool _started;
    bool _joined;//该变量的作用是记录是否已经调用过pthread_join函数
    pthread_t _pthreadId;
    std::shared_ptr<pid_t > _tid;
    ThreadFunc _func;
};


#endif