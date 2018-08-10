#ifndef ECHO_H
#define ECHO_H

#include "../Server.h"




class EchoServer
{
public:
    EchoServer(EventLoop* loop,const InetAddr& listenAddr);

    void  start();

private:
    void OnConnection(const ConnectionPtr& conn);
    void OnMessage(const ConnectionPtr& conn,
                Buffer* buffer,
                TimeUnit timeUnit);

    EventLoop* _loop;
    Server _server;
};
#endif