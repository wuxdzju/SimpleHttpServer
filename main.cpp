#include <errno.h>
#include "threadpool.h"
#include "epoll.h"
#include "http_tast.h"
#include "myerr.h"


const int MAX_FD = 20;
const int MAX_EVENT_NUM=10000;



int main(int argc,char* argv[]) {
    if(argc<=2){
        printf("Usage: %s ip_adress port\n",argv[0]);
        return 1;
    }

    const char *ip=argv[1];
    const char *port=argv[2];

    //忽略SIGPIPE信号
    add_sig_hand(SIGPIPE,SIG_IGN);

    //创建线程池
    threadpool<http_tast> *pool=NULL;
    try{
        pool=new threadpool<http_tast>;
    }
    catch (...){
        return 1;
    }

    //预先为每个可能的客户分配http_tast对象
    http_tast *tasts=new http_tast[MAX_FD];
    if(tasts==NULL){
        err_sys("main:new http_tast err:\n");
    }

    //创建监听socket，并监听
    int listenfd=open_listenfd(ip,port);

    //创建epoll,并将监听socket添加到epoll_events中
    epoll_event events[MAX_EVENT_NUM];
    int epollfd=epoll_create(10);
    if(epollfd<0){
        err_sys("main:epoll_create err:");
    }
    addfd(epollfd,listenfd,false);
    http_tast::m_epollfd=epollfd;

    while(true){
        int num=epoll_wait(epollfd,events,MAX_EVENT_NUM,-1);
        if(num<0 && errno!=EINTR){
            err_ret("main:epoll_wait err:");
            break;
        }

        for(int i=0;i<num;i++){
            int sockfd=events[i].data.fd;
            if(sockfd==listenfd){
                struct sockaddr_in client_addr;
                socklen_t client_len= sizeof(client_addr);

                int connfd=accept(listenfd,(struct sockaddr *)&client_addr,&client_len);
                if(connfd<0){
                    err_ret("main:accept err:");
                    continue;
                }
                if(http_tast::m_user_cnt>=MAX_FD){
                    show_err(connfd,"Internal server busy");
                    continue;
                }
                //初始化客户连接
                printf("connected fd:%d\n",connfd);
                tasts[connfd].init(connfd,client_addr);
            }
            else if(events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)){
                //有异常，关闭连接
                tasts[sockfd].close_conn();
            }
            else if(events[i].events & EPOLLIN){
                tasts[sockfd].m_ready_status=READY_READ;
                pool->add(tasts+sockfd);
            }
            else if(events[i].events & EPOLLOUT){
                tasts[sockfd].m_ready_status=READY_WRITE;
                pool->add(tasts+sockfd);
            }
        }
    }
    close(epollfd);
    close(listenfd);
    delete [] tasts;
    delete pool;
    return 0;
}