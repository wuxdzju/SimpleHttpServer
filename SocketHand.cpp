#include "SocketHand.h"
#include <errno.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>

const SocketHand::SA* SocketHand::sockaddr_cast(const struct sockaddr_in* addr)
{
    return static_cast<const SA*>(static_cast<const void*>(addr));
}

SocketHand::SA* SocketHand::sockaddr_cast(struct sockaddr_in* addr)
{
    return static_cast<SA*>(static_cast<void*>(addr));
}

uint16_t SocketHand::hostToNet16(uint16_t host16)
{
    return htons(host16);
}
uint16_t SocketHand::netToHost16(uint16_t net16)
{
    return ntohs(net16);
}

uint32_t SocketHand::hostToNet32(uint32_t host32)
{
    return htonl(host32);
}

uint32_t SocketHand::netToHost32(uint32_t net32)
{
    return ntohl(net32);
}


void SocketHand::setNonBlockAndCloseOnExec(int sockfd)
{
    //将sockfd设置为nonblock和close-on-exec的
    int flag=::fcntl(sockfd,F_GETFD,0);
    flag |= O_NONBLOCK;
    flag |= O_CLOEXEC;
    int ret=::fcntl(sockfd,F_SETFL,flag);
    if(ret<0)
    {
        perror("SocketHand::setNonBlockAndCloseOnExec():");
    }
}

int SocketHand::createNonBlockingOrDie()
{
    int sockfd=::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sockfd<0)
    {
        perror("SocketHand::createNonBlockingOrDie():");
        _exit(1);
    }

    setNonBlockAndCloseOnExec(sockfd);

    return sockfd;
}

int SocketHand::createBlockingOrDie()
{
    int sockfd=::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sockfd<0)
    {
        perror("SocketHand::createNonBlockingOrDie():");
        _exit(1);
    }

    return sockfd;
}

void SocketHand::bindOrDie(int sockfd,const struct sockaddr_in& addr)
{
    int ret=::bind(sockfd,sockaddr_cast(&addr), sizeof(addr));
    if(ret<0)
    {
        perror("SocketHand::bindOrDie():");
        _exit(1);
    }
}

void SocketHand::listenOrDie(int sockfd)
{
    int ret=::listen(sockfd,SOMAXCONN);
    if(ret<0)
    {
        perror("SocketHand::listenOrDie():");
        _exit(1);
    }
}

int SocketHand::acceptConn(int sockfd, struct sockaddr_in* addr)
{
    socklen_t socklen= sizeof(*addr);
    int connfd=::accept(sockfd,sockaddr_cast(addr),&socklen);
    setNonBlockAndCloseOnExec(connfd);

    int saveErrno=errno;
    if(connfd<0)
    {
        switch (saveErrno)
        {
            case EAGAIN:
            case EINTR:
            case ECONNABORTED:
                errno=saveErrno;
                break;
            default:
                //对于listenfd为非阻塞fd，且没用IO复用，直接调用的话，会终止进程
                perror("SocketHand::acceptConn():");
                _exit(1);
        }
    }

    return connfd;
}

void SocketHand::closeFd(int sockfd)
{
    int ret=::close(sockfd);
    if(ret<0)
    {
        perror("SocketHand::closeFd():");
    }
}

void SocketHand::hostPortToSockAddr(const char *ip,uint16_t port, struct sockaddr_in *addr)
{
    addr->sin_family=AF_INET;
    addr->sin_port=hostToNet16(port);
    if(::inet_pton(AF_INET,ip,&addr->sin_addr)<=0)
    {
        perror("SocketHand::hostPortToSockAddr():");
    }
}

void SocketHand::sockAddrToHostPort(const struct sockaddr_in *addr,char* buf,size_t size)
{
    char host[INET_ADDRSTRLEN]="INVALID";
    ::inet_ntop(AF_INET,&addr->sin_addr,host, sizeof(host));
    uint16_t port=netToHost16(addr->sin_port);
    sprintf(buf,"%s:%u",host,port);
}


struct sockaddr_in SocketHand::getLocalAddr(int sockfd)
{
    struct sockaddr_in localaddr;
    bzero(&localaddr, sizeof(localaddr));
    socklen_t addrlen= sizeof(localaddr);
    if(::getsockname(sockfd,sockaddr_cast(&localaddr),&addrlen)<0)
    {
        perror("SocketHand::getLocalAddr()");
    }
    return localaddr;
}