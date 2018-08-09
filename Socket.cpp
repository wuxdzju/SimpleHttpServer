#include "Socket.h"
#include "SocketHand.h"

#include <iostream>

Socket::~Socket()
{
    SocketHand::closeFd(_sockfd);
    //std::cout<<"close fd"<<std::endl;
}

void Socket::bindAddrr(const InetAddr &localAddr)
{
    SocketHand::bindOrDie(_sockfd,localAddr.getSockAddr());
}

void Socket::listen()
{
    SocketHand::listenOrDie(_sockfd);
}

int Socket::acceptConn(InetAddr *peerAddr)
{
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    int connfd=::SocketHand::acceptConn(_sockfd,&addr);
    if(connfd>=0)
    {
        peerAddr->setSockAddr(addr);
    }
    return connfd;
}

void Socket::setReuseAddr(bool on)
{
    int opval=on?1:0;
    ::setsockopt(_sockfd,SOL_SOCKET,SO_REUSEADDR,&opval,sizeof(opval));
}

void Socket::setReusePort(bool on)
{
    int opval=on?1:0;
    ::setsockopt(_sockfd,SOL_SOCKET,SO_REUSEADDR,&opval,sizeof(opval));
}

void Socket::shutdownWrite()
{
    SocketHand::shutdownWrite(_sockfd);
}
