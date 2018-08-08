#include "Connection.h"
#include "Socket.h"
#include "Channel.h"
#include "InetAddr.h"
#include "assert.h"
#include "EventLoop.h"

#include <unistd.h>
#include <functional>
#include <iostream>

using std::placeholders::_1;


Connection::Connection(EventLoop *loop,
                       const std::string name,
                       int socket,
                       const InetAddr &localAddr,
                       const InetAddr &peerAddr)
        :_loop(loop),
         _name(name),
         _connState(D_CONNECTING),
         _socket(new Socket(socket)),
         _channel(new Channel(loop,socket)),
         _localAddr(localAddr),
         _peerAddr(peerAddr)
{
    //Connection对象拥有_socket和_channel
    _channel->setReadCallBack(std::bind(&Connection::handRead,this,_1));
    _channel->setWriteCallBack(std::bind(&Connection::handWrite,this));
    _channel->setErrorCallBack(std::bind(&Connection::handError,this));
    _channel->setCloseCallBack(std::bind(&Connection::handClose,this));
}

Connection::~Connection()
{

}

void Connection::ConnectEstablished()
{
    _loop->assertInLoopThread();
    assert(_connState==D_CONNECTING);
    setConnState(D_CONNECTED);
    _channel->enableReading();

    _connectionCallback(shared_from_this());
}

void Connection::handRead(TimeUnit recieveTime)
{

    int savedErrno=0;
    ssize_t  n=_inputBuffer.readFd(_channel->fd(),&savedErrno);
    if(n>0)
    {
        _messageCallback(shared_from_this(),&_inputBuffer,recieveTime);
    }
    else if(n==0)
    {
        handClose();
    }
    else
    {
        errno=savedErrno;
        handError();
    }

}

void Connection::handClose()
{
    _loop->assertInLoopThread();
    assert(_connState==D_CONNECTED);

    //在这里并不急于关闭文件描述符，只是禁止该文件描述符上所有的事件，等到析构时，再关闭
    _channel->disableAllEvent();

    //这里调用Server中的函数，用于将该Connection从connMap中移除
    _closeCallback(shared_from_this());
}

void Connection::handError()
{
    //todo 打印错误信息
    std::cout<<"Connection::handError()["<<_name<<"]"<<std::endl;
}

void Connection::connectionDestroyed()
{
    _loop->assertInLoopThread();
    assert(_connState==D_CONNECTED);

    setConnState(D_DISCONNECTED);
    _channel->disableAllEvent();
    _connectionCallback(shared_from_this());

    _loop->removeChannel(_channel.get());
}

void Connection::handWrite()
{

}