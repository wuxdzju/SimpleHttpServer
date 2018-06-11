#ifndef LOCK_H
#define LOCK_H

#include <exception>
#include <pthread.h>
#include <semaphore.h>

class sem{
public:
    sem(){
        if(sem_init(&m_sem,0,0)!=0){
            throw std::exception();
        }
    }

    ~sem(){
        sem_destroy(&m_sem);
    }

    bool P(){
        return sem_wait(&m_sem)==0;
    }

    bool V(){
        return sem_post(&m_sem)==0;
    }

private:
    sem_t m_sem;
};

class mutex{
public:
    mutex(){
        if(pthread_mutex_init(&m_mutex,NULL)!=0){
            throw std::exception();
        }
    }

    ~mutex(){
        pthread_mutex_destroy(&mutex);
    }

    bool lock(){
        return pthread_mutex_lock(&m_mutex)==0;
    }

    bool unlock(){
        return pthread_mutex_unlock(&m_mutex)==0;
    }

private:
    pthread_mutex_t m_mutex;
};

class cond{
public:
    cond(){
        if(pthread_mutex_init(&m_mutex,NULL)!=0){
            throw std::exception();
        }

        if(pthread_cond_init(&m_cond,NULL)!=0){
            //构造函数如果出现问题的话，之前分配的资源应当要释放掉
            pthread_mutex_destroy(&m_mutex);
            throw std::exception();
        }
    }

    ~cond(){
        pthread_mutex_destroy(&m_mutex);
        pthread_cond_destroy(&m_cond);
    }

    bool wait(){
        int ret;
        pthread_mutex_lock(&m_mutex);
        ret=pthread_cond_wait(&m_cond,&mutex);
        pthread_mutex_unlock(&m_mutex);
        return ret;
    }

    bool signal(){
        return pthread_cond_signal(&m_cond)==0;
    }

private:
    pthread_cond_t m_cond;
    pthread_mutex_t m_mutex;
};


#endif