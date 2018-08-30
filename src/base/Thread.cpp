#include "Thread.h"
#include "CurrentThread.h"
#include <unistd.h>
#include <syscall.h>
#include <assert.h>



namespace CurrentThread
{
    __thread int t_cachedTid = 0;
}

pid_t gettid()
{
    return static_cast<pid_t>(::syscall(SYS_gettid));
}

void CurrentThread::cacheTid()
{
    if (t_cachedTid == 0)
    {
        t_cachedTid = gettid();
    }
}

//ThreadData的作用是在线程中保留tid这些数据
struct ThreadData
{
    typedef Thread::ThreadFunc ThreadFunc;
    ThreadFunc _func;
    std::weak_ptr<pid_t> _wkTid;

    ThreadData(const ThreadFunc& func,
                const std::shared_ptr<pid_t> &tid)
            :_func(func),_wkTid(tid)
    {

    }

    void runInThread()
    {
        pid_t curtid=CurrentThread::tid();
        std::shared_ptr<pid_t > ptid=_wkTid.lock();

        if(ptid)
        {
            *ptid=curtid;
            ptid.reset();
        }
        //调用传进来的函数
        _func();
    }
};

//线程的启动函数
void* startThread(void *obj)
{
    ThreadData* data= static_cast<ThreadData*>(obj);
    data->runInThread();//调用ThreadData中的启动函数
    delete data;
    return nullptr;
}

Thread::Thread(const ThreadFunc &func)
        :_joined(false),
         _started(false),
         _pthreadId(0),
         _tid(new pid_t(0)),
         _func(func)
{

}

Thread::~Thread()
{
    //只有在该线程没有处于_started状态，以及没有对该线程调用过pthread_join函数时，
    // 才会调用pthread_detach函数，分离线程，在线程离开时自动释放线程资源
    if(_started && !_joined)
    {
        pthread_detach(_pthreadId);
    }
}

void Thread::start()
{
    assert(!_started);
    _started= true;

    ThreadData *pdata=new ThreadData(_func,_tid);
    if(pthread_create(&_pthreadId, nullptr,startThread,pdata))
    {
        _started= false;
        delete pdata;
    }
}

int Thread::join()
{
    assert(_started);
    assert(!_joined);
    _joined= true;
    pthread_join(_pthreadId, nullptr);
}




