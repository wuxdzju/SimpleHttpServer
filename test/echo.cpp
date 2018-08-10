#include "echo.h"
#include "../EventLoop.h"

#include <iostream>

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

EchoServer::EchoServer(EventLoop *loop, const InetAddr &listenAddr)
        :_loop(loop),
         _server(loop,listenAddr)
{
    _server.setConnectionCallBack(std::bind(&EchoServer::OnConnection,this,_1));
    _server.setMessageCallBack(std::bind(&EchoServer::OnMessage,this,_1,_2,_3));
}


void EchoServer::OnConnection(const ConnectionPtr &conn)
{
    std::cout<<"EchoServer - "<<conn->getPeerAddr().toHostPort()<<" -> "
             <<conn->getLocalAddr().toHostPort()<<" is "<<(conn->Connected()?"UP":"DOWN")<<std::endl;//endl可以刷新标准缓冲区

}

void EchoServer::OnMessage(const ConnectionPtr &conn, Buffer *buffer, TimeUnit timeUnit)
{
    std::string msg(buffer->retrieveAsString());
    std::cout<<conn->getName()<<" echo "<<msg.size()<<" bytes,"
              <<"data receive at "<<timeUnit.ToString()<<std::endl;//endl可以刷新标准输出的缓冲区
    conn->send(msg);
}

void EchoServer::start()
{
    _server.start();
}

