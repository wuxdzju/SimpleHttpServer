#ifndef EPOLL_H
#define EPOLL_H

#include <fcntl.h>
#include <sys/epoll.h>

#include "myerr.h"
#include "util.h"

int setnoblock(int fd);//设置为非阻塞
int addfd(int efd,int fd,bool one_shot);//向epoll中添加文件描述符
int removefd(int efd,int fd);//删除epoll中的文件描述符
int modifyfd(int efd,int fd,int ev);//更改epoll中某个文件描述符的事件

#endif