#ifndef ERROR_H
#define ERROR_H

#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

/*错误处理函数*/
static void error_doit(int errnoflag,int error,const char *msg,va_list ap);//打印错误信息，并返回调用函数,errnoflag参数用于是否从strerror添加字符串
void err_ret(const char *msg,...);//系统调用的非致命错误，打印信息并return
void err_sys(const char *msg,...);//系统调用的致命错误，打印信息并终止
void err_cont(int error,const char *msg,...);//非系统调用的非致命错误，第一个参数为错误代码，打印信息并返回
void err_exit(int error,const char *msg,...);//非系统调用的致命错误，打印信息并终止
void err_dump(const char *msg,...);//系统调用的致命错误，打印信息、核心转储终止
void err_msg(const char *msg,...);//非系统调用的非致命错误，打印信息并返回
void err_quit(const char *msg,...);//非系统调用的致命错误，打印信息并终止

#endif