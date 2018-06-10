#ifndef THREADPOLL_H
#define THREADPOLL_H

#include <list>
#include <exception>
#include <pthread.h>


template <typename T>
class threadpool{
public:
    threadpool(int thread_num=4,int max_request_num=10000);
    ~threadpool();
    bool add(T* request);//往请求队列中添加任务
private:
    static void* worker(void *arg);//工作线程运行的函数
    void run();
    bool lock();
    bool unlock();
    bool wait();
    bool signal();

private:
    int m_thread_num;//工作线程数量
    int m_max_request_num;//请求队列中允许的最大请求数
    pthread_t *m_threads;//指向线程数组头元素的指针
    std::list<T*> m_workqueue;//请求队列
    pthread_mutex_t m_queuemutex;//保护请求队列的互斥锁
    pthread_cond_t m_queuecond;//请求队列上是否有任务
    bool m_stop;//是否结束线程
};

template <typename T>
threadpool<T>::threadpool(int thread_num, int max_request_num) :
        m_thread_num(thread_num),m_max_request_num(max_request_num),
        m_stop(false),m_threads(NULL) {
    if(thread_num<=0 || max_request_num<=0){
        throw std::exception();
    }

    if(pthread_mutex_init(&m_queuemutex,NULL)!=0){
        throw std::exception();
    }

    if(pthread_cond_init(&m_queuecond,NULL)!=0){
        pthread_mutex_destroy(&m_queuemutex);
        throw std::exception();
    }

    m_threads=new pthread_t[m_thread_num];
    if(!m_threads){
        throw std::exception();
    }

    //创建m_thread_num个线程，并将其设置为detach性质
    for(int i=0;i<m_thread_num;i++){
        if(pthread_create(m_threads+i,NULL,worker,this)!=0){
            delete [] m_threads;
            throw std::exception();
        }

        if(pthread_detach(m_threads[i])!=0){
            delete [] m_threads;
            throw std::exception();
        }
    }
}

template <typename  T>
threadpool<T>::~threadpool() {
    delete[] m_threads;
    m_stop= true;
}

template <typename T>
bool threadpool<T>::lock() {
    return pthread_mutex_lock(&m_queuemutex)==0;
}

template <typename T>
bool threadpool<T>::unlock() {
    return pthread_mutex_unlock(&m_queuemutex)==0;
}

template <typename T>
bool threadpool<T>::wait() {
    return pthread_cond_wait(&m_queuecond,&m_queuemutex)==0;
}

template <typename T>
bool threadpool<T>::signal() {
    return pthread_cond_signal(&m_queuecond);
}


template <typename T>
bool threadpool<T>::add(T *request) {
    //改变请求队列前要记得加锁
    lock();
    if(m_workqueue.size()>=m_max_request_num){
        unlock();
        return false;
    }
    m_workqueue.push_back(request);
    signal();
    unlock();
    return true;
}

template <typename T>
void* threadpool<T>::worker(void *arg) {
    threadpool * pool=(threadpool*)arg;
    pool->run();
    return pool;
}

template <typename T>
void threadpool<T>::run() {
    while(!m_stop){
        lock();
        while(m_workqueue.empty()){
            wait();
        }
        if(m_workqueue.empty()){
            unlock();
            continue;
        }
        T* request=m_workqueue.front();
        m_workqueue.pop_front();
        unlock();
        if(!request){
            continue;
        }
        request->process();//调用任务节点的处理函数
    }
}

#endif