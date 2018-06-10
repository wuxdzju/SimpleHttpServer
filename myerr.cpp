#include "myerr.h"
#include <string.h>
#include <stdlib.h>

const  int MAXLINE=1024;



static void error_doit(int errnoflag,int error,const char *msg,va_list ap){
    char buf[MAXLINE];

    vsnprintf(buf,MAXLINE-1,msg,ap);

    if(errnoflag){
        snprintf(buf+strlen(buf),MAXLINE-strlen(buf)-1,":%s",strerror(error));
    }
    strcat(buf,"\n");
    fflush(stdout);//刷新标准输出，防止stdout和stderr是同一个
    fputs(buf,stderr);
    fflush(NULL);//刷新所有的输出流
}//打印错误信息，并返回调用函数,errnoflag参数用于是否从strerror添加字符串


void err_ret(const char *msg,...){
    va_list ap;

    va_start(ap,msg);
    error_doit(1,errno,msg,ap);
    va_end(ap);
}//系统调用的非致命错误，打印信息并return

void err_sys(const char *msg,...){
    va_list ap;

    va_start(ap,msg);
    error_doit(1,errno,msg,ap);
    va_end(ap);
    exit(1);
}//系统调用的致命错误，打印信息并终止


void err_cont(int error,const char *msg,...){
    va_list ap;

    va_start(ap,msg);
    error_doit(1,error,msg,ap);
    va_end(ap);
}//非系统调用的非致命错误，第一个参数为错误代码，打印信息并返回


void err_exit(int error,const char *msg,...){
    va_list ap;

    va_start(ap,msg);
    error_doit(1,error,msg,ap);
    va_end(ap);
    exit(1);
}//非系统调用的致命错误，打印信息并终止


void err_dump(const char *msg,...){
    va_list ap;

    va_start(ap,msg);
    error_doit(1,errno,msg,ap);
    va_end(ap);
    abort();//核心转储并且终止
    exit(1);//其实并不会执行到这里
}//系统调用的致命错误，打印信息、核心转储终止


void err_msg(const char *msg,...){
    va_list ap;

    va_start(ap,msg);
    error_doit(0,0,msg,ap);
    va_end(ap);
}//非系统调用的非致命错误，打印信息并返回


void err_quit(const char *msg,...){
    va_list ap;

    va_start(ap,msg);
    error_doit(0,0,msg,ap);
    va_end(ap);
    exit(1);
}//非系统调用的致命错误，打印信息并终止

