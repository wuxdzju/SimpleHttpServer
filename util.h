#ifndef UTIL_H
#define UTIL_H

#include <signal.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "myerr.h"

enum HTTP_CODE { NO_REQUEST, GET_REQUEST, BAD_REQUEST, NO_RESOURCE, FORBIDDEN_REQUEST, FILE_REQUEST, INTERNAL_ERROR, CLOSED_CONNECTION };
enum READY_STATUS{READY_NO,READY_READ,READY_WRITE};

void add_sig_hand(int sig,void(handler)(int),bool restart= true);//注册信号处理函数
int open_listenfd(const char *host,const char* port);
void show_err(int connfd,const char* info);



#endif