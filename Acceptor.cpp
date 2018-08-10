#include "Acceptor.h"
#include "SocketHand.h"
#include "InetAddr.h"
#include "EventLoop.h"


Acceptor::Acceptor(EventLoop* loop,const InetAddr& listenAddr)
        :_loop(loop),
         _acceptSocket(SocketHand::createNonBlockingOrDie()),
         _acceptChannel(loop,_acceptSocket.fd()),
        _listening(false)
{
    _acceptSocket.bindAddrr(listenAddr);
    _acceptSocket.setReuseAddr(true);
    //_acceptSocket.setReusePort(true);

    _acceptChannel.setReadCallBack(std::bind(&Acceptor::handRead,this));
}

void Acceptor::listen()
{
    _loop->assertInLoopThread();
    _listening= true;
    _acceptSocket.listen();
    _acceptChannel.enableReading();
}

void Acceptor::handRead()
{
    _loop->assertInLoopThread();
    InetAddr peerAddr(0);
    int connfd=_acceptSocket.acceptConn(&peerAddr);
    if(connfd>=0)
    {
        if(_newConnCallBack)
        {
            _newConnCallBack(connfd,peerAddr);
        }
        else
        {
            SocketHand::closeFd(connfd);
        }
    }
}