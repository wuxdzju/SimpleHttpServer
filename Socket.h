#ifndef SOCKET_H
#define SOCKET_H

#include "base/noncopyable.h"
#include "InetAddr.h"

class Socket:noncopyable
{
public:
    explicit Socket(int sockfd)
            :_sockfd(sockfd)
    {

    }

    ~Socket();

    int fd() const
    {
        return _sockfd;
    }

    void bindAddrr(const InetAddr& localAddr);

    void listen();

    int acceptConn(InetAddr* peerAddr);

    void setReuseAddr(bool on);

    void setReusePort(bool on);

private:
    const int _sockfd;
};

#endif