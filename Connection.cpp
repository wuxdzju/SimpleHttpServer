#include "Connection.h"
#include "Socket.h"
#include "Channel.h"
#include "InetAddr.h"
#include "assert.h"
#include "EventLoop.h"
#include "base/WeakCallback.h"


#include <unistd.h>
#include <functional>
#include <iostream>

using std::placeholders::_1;
extern const int timeOutSeconds;

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
         _peerAddr(peerAddr),
         _lastActiveTime(TimeUnit::now())
{
    //Connection对象拥有_socket和_channel
    _channel->setReadCallBack(std::bind(&Connection::handRead,this,_1));
    _channel->setWriteCallBack(std::bind(&Connection::handWrite,this));
    _channel->setErrorCallBack(std::bind(&Connection::handError,this));
    _channel->setCloseCallBack(std::bind(&Connection::handClose,this));
}

Connection::~Connection()
{
    std::cout<<"Connection::~Connection()"<<std::endl;
    assert(_connState == D_DISCONNECTED);
}

void Connection::ConnectEstablished()
{
    _loop->assertInLoopThread();
    assert(_connState==D_CONNECTING);
    setConnState(D_CONNECTED);
    setLastActiveTime(TimeUnit::now());
    _channel->enableReading();

    _connectionCallback(shared_from_this());
}

void Connection::handRead(TimeUnit receiveTime)
{
    _loop->assertInLoopThread();
    //std::cout<<timeOutSeconds<<std::endl;
    //更新Connection的_lastActivetime
    //setLastActiveTime(receiveTime);
    //forceCloseWithDelay(timeOutSeconds);

    int savedErrno=0;
    ssize_t  n=_inputBuffer.readFd(_channel->fd(),&savedErrno);
    if(n>0)
    {
        _messageCallback(shared_from_this(),&_inputBuffer,receiveTime);
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
    assert(_connState==D_CONNECTED || _connState==D_DISCONNECTING);

    //在这里并不急于关闭文件描述符，只是禁止该文件描述符上所有的事件，等到析构时，再关闭
    _channel->disableAllEvent();

    //这里调用Server中的函数，用于将该Connection从connMap中移除
    _closeCallback(shared_from_this());
}

void Connection::handError()
{
    //todo 打印错误信息
    _loop->assertInLoopThread();
    std::cout<<"Connection::handError()["<<_name<<"]"<<std::endl;
}

void Connection::connectionDestroyed()
{
    _loop->assertInLoopThread();
    assert(_connState==D_CONNECTED || _connState==D_DISCONNECTING);

    setConnState(D_DISCONNECTED);
    _channel->disableAllEvent();
    _connectionCallback(shared_from_this());

    _loop->removeChannel(_channel.get());
}


void Connection::shutdown()
{
    if(_connState==D_CONNECTED)
    {
        setConnState(D_DISCONNECTING);
        _loop->runInLoopThread(std::bind(&Connection::shutdownInLoop,this));
    }
}

void Connection::shutdownInLoop()
{
    _loop->assertInLoopThread();
    if(!_channel->isWriting())
    {
        //当不在写的时候，才进行关闭
        _socket->shutdownWrite();
    }
}

void Connection::send(const std::string &message)
{
    if(_connState==D_CONNECTED)
    {
        if(_loop->isInLoopThread())
        {
            sendInLoop(message);
        }
        else
        {
            _loop->runInLoopThread(std::bind(&Connection::sendInLoop,this,message));
        }
    }
}

void Connection::send(Buffer *buf)
{
    if(_connState == D_CONNECTED)
    {
        if(_loop->isInLoopThread())
        {
            sendInLoop(buf->retrieveAsString());
        }
        else
        {
            _loop->runInLoopThread(
                    std::bind(&Connection::sendInLoop, this, buf->retrieveAsString()));
        }
    }
}

void Connection::sendInLoop(const std::string &message)
{
    _loop->assertInLoopThread();
    ssize_t nwrote=0;

    if(!_channel->isWriting() && _outputBuffer.readableBytes()==0)
    {
        nwrote=::write(_channel->fd(),message.data(),message.size());

        if(nwrote<0)
        {
            nwrote=0;
            if(errno!=EWOULDBLOCK)
            {
                perror("Connection::sendInLoop():");
            }
        }

        assert(nwrote>=0);
        //如果一次没有将数据发送完，则将剩余数据添加到_outBuffer中，并并注册读事件
        if(static_cast<size_t >(nwrote)<message.size())
        {
            _outputBuffer.append(message.data()+nwrote,message.size()-nwrote);
            if(!_channel->isWriting())
            {
                _channel->enableWriting();
            }
        }
    }
}


void Connection::handWrite()
{
    _loop->assertInLoopThread();
    if(_channel->isWriting())
    {
        ssize_t nwrote=::write(_channel->fd(),_outputBuffer.peek(),_outputBuffer.readableBytes());

        if(nwrote>0)
        {
            _outputBuffer.retrieve(nwrote);
            if(_outputBuffer.readableBytes()==0)
            {
                _channel->disableWriting();
                if(_connState==D_DISCONNECTING)
                {
                    shutdownInLoop();
                }
            }
            else
            {
                printf("i am going to write more data");
            }
        }
        else
        {
            perror("Connection::handWrite():");
        }
    }
    else
    {
        printf("Connection is down,no more writing");
    }
}

void Connection::forceClose()
{
    if(_connState == D_CONNECTED || _connState == D_DISCONNECTING)
    {
        setConnState(D_DISCONNECTING);
        _loop->queueInLoop(
                std::bind(&Connection::forceCloseInLoop,shared_from_this()));
    }
}

bool Connection::canDeleted() const
{
    TimeUnit expiredTime = addTime(_lastActiveTime, timeOutSeconds);
    return expiredTime <= TimeUnit::now();
}

void Connection::forceCloseWithDelay(int seconds)
{
    if(_connState == D_CONNECTED || _connState == D_DISCONNECTING)
    {
        _loop->runAfter(seconds,
                        makeWeakCallback(shared_from_this(),
                                         &Connection::forceClose),
                        getLastActiveTime());//不直接调用forceCloseInLoop()的原因是为了避免竞争
                                                                    //因为这是一个延迟调用的函数，可能在seconds之后，
                                                                    //该连接可能已经被动关闭了，此时直接调用forceCloseInLoop()会发生竞争，因为该连接已经析构了
    }
}

void Connection::forceCloseInLoop()
{
    //std::cout<<"Connection::forceCloseInLoop()"<<std::endl;
    _loop->assertInLoopThread();
    if(!canDeleted())
        return;

    if(_connState == D_CONNECTED ||_connState == D_DISCONNECTING)
    {
        handClose();
    }
}