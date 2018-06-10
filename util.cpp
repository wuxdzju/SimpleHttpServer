#include "util.h"

const int MAXLINE2=1024;
const int LISTEN_QUEUE=20;

void add_sig_hand(int sig,void(handler)(int),bool restart){
    struct sigaction sa;
    memset(&sa,'\0', sizeof(sa));
    sa.sa_handler=handler;
    sigfillset(&sa.sa_mask);//处理该信号时，屏蔽所有的信号
    if(restart){
        sa.sa_flags |= SA_RESTART;//只要可能，被中断的系统调用可被重新启用
    }
    if(sigaction(sig,&sa,NULL)==-1){
        err_sys("add_sig_hand:sigaction err:");
    }
}

int open_listenfd(const char *host,const char* port){
    int listenfd,optval=1;
    struct addrinfo hints,*listp,*p;

    //获得一个可能的服务器地址列表
    memset(&hints,0,sizeof(struct addrinfo));
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags=AI_PASSIVE | AI_ADDRCONFIG |AI_NUMERICSERV | AI_NUMERICHOST;
    int gai_ret=getaddrinfo(host,port,&hints,&listp);
    if(gai_ret){
        char tmp_err[]="open_listen:getaddrinfo err:";
        strncpy(tmp_err,gai_strerror(gai_ret),MAXLINE2-strlen(tmp_err)-1);
        err_quit(tmp_err);
    }

    //对服务器地址中的地址结构列表逐一进行bind尝试
    for(p=listp;p;p=p->ai_next){
        //创建一个套接字描述符
        if((listenfd=socket(p->ai_family,p->ai_socktype,p->ai_protocol))<0){
            continue;
        }

        //设置套接字选项，使得服务器能够被终止、重启和立即开始接受连接请求
        if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(int))<0){
            continue;
        }

        if(bind(listenfd,p->ai_addr,p->ai_addrlen)==0)//成功
            break;
        close(listenfd);
    }

    freeaddrinfo(listp);

    if(!p)//所有的地址都创建失败
        return -1;

    //将套接字转换为监听套接字
    if(listen(listenfd,LISTEN_QUEUE)<0){
        close(listenfd);
        return -1;
    }
    return listenfd;
}

void show_err(int connfd,const char* info){
    printf("%s",info);
    send(connfd,info,strlen(info),0);
    close(connfd);
}