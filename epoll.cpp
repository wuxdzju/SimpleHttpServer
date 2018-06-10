#include "epoll.h"
#include <unistd.h>


int setnoblock(int fd){
    int old_op=fcntl(fd,F_GETFL);
    if(old_op==-1){
        err_ret("setnoblock:fcntl err:");
        return -1;
    }
    int new_op=old_op | O_NONBLOCK;
    int ret=fcntl(fd,F_SETFL,new_op);
    if(ret==-1){
        err_ret("setnoblock:fcntl err:");
        return -1;
    }
    return old_op;
};//设置为非阻塞

int addfd(int efd,int fd,bool one_shot){
    setnoblock(fd);
    epoll_event event;
    event.data.fd=fd;
    event.events=EPOLLIN | EPOLLET | EPOLLERR;
    if(one_shot){
        event.events |= EPOLLONESHOT;
    }
    int ret=epoll_ctl(efd,EPOLL_CTL_ADD,fd,&event);
    if(ret==-1){
        err_ret("addfd:epoll_ctl err:");
        return -1;
    }

};//向epoll中添加文件描述符

int removefd(int efd,int fd){
    int ret=epoll_ctl(efd,EPOLL_CTL_DEL,fd,0);
    close(fd);
    if(ret==-1){
        err_ret("removefd:epoll_ctl err:");
        return -1;
    }
};//删除epoll中的文件描述符

int modifyfd(int efd,int fd,int ev){
    epoll_event event;
    event.data.fd=fd;
    event.events=ev | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
    int ret=epoll_ctl(efd,EPOLL_CTL_MOD,fd,&event);
    if(ret==-1){
        err_ret("modifyfd:epoll_ctl err:");
        return -1;
    }
};//更改epoll中某个文件描述符的事件