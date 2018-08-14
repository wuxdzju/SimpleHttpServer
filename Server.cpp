#include "Server.h"
#include "Acceptor.h"
#include "EventLoop.h"
#include "EventLoopThreadPool.h"

#include <memory>
#include <functional>

using std::placeholders::_1;
using std::placeholders::_2;

 Server::Server(EventLoop *loop, const InetAddr &listenAddr)
         :_loop(loop),
          _name(listenAddr.toHostPort()),
          _acceptor(new Acceptor(loop,listenAddr)),
          _threadpool(new EventLoopThreadPool(loop)),
          _started(false),
          _nextConnId(1)
 {
     //_acceptor的生命期由Server控制
     //在新连接到达时，_acceptor会回调newConnection
     _acceptor->setNewConnCallBack(std::bind(&Server::newConnection,this,_1,_2));
 }

Server::~Server()
{

}

void Server::setThreadNum(int threadNum)
{
    _threadpool->setThreadNum(threadNum);
}

void Server::start()
{
    if(!_started)
    {
        _started= true;
        _threadpool->start();
    }

    if(!_acceptor->isListening())
    {
        //调用_acceptor中的listen函数
        //_acceptor.get()获取智能指针所拥有的原始指针
        _loop->runInLoopThread(std::bind(&Acceptor::listen,_acceptor.get()));
    }
}

//newConnection会创建Connection对象conn，把它加入到connectionMap中，设置好回调函数
//NewConnection函数由_acceptor回调
void Server::newConnection(int sockfd, const InetAddr &peerAddr)
{
    _loop->assertInLoopThread();
    //生成Connection的Name后缀，每个Connection独一无二
    char buf[32];
    snprintf(buf, sizeof buf,"#%d",_nextConnId);
    ++_nextConnId;
    std::string connName=_name+buf;

    //获取本地地址
    InetAddr localaddr(SocketHand::getLocalAddr(sockfd));

    //创立一个新的连接，将其加入到server管理的连接map中，设置好该连接的回调函数，并且让该链接处于可读状态
    EventLoop *ioLoop = _threadpool->getNextLoop();
    ConnectionPtr conn(
            new Connection(ioLoop,connName,sockfd,localaddr,peerAddr));
    _connections[connName]=conn;

    conn->setConnectionCallback(_connectionCallBack);
    conn->setMessageCallback(_messageCallBack);
    conn->setCloseCallback(std::bind(&Server::removeConnection,this,_1));
    //conn->ConnectEstablished();
    ioLoop->runInLoopThread(
            std::bind(&Connection::ConnectEstablished,conn));
}

void Server::removeConnection(const ConnectionPtr &conn)
{
    _loop->runInLoopThread(
            std::bind(&Server::remmoveConnectionInLoop,this,conn));
}

void Server::remmoveConnectionInLoop(const ConnectionPtr &conn)
{
    _loop->assertInLoopThread();

    size_t  n=_connections.erase(conn->getName());
    assert(n==1);

    EventLoop* ioLoop = conn->getLoop();

    ioLoop->queueInLoop(
            std::bind(&Connection::connectionDestroyed,conn));
}

