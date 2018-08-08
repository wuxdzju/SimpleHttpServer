#ifndef INETADDR_H
#define INETADDR_H

#include <netinet/in.h>
#include <string>

class InetAddr
{
public:
    explicit InetAddr(uint16_t port);

    InetAddr(const std::string &ip,uint16_t port);

    InetAddr(const struct sockaddr_in addr)
            :_addr(addr)
    {

    }

    std::string toHostPort() const;

    const struct sockaddr_in& getSockAddr() const
    {
        return _addr;
    }

    void setSockAddr(const struct sockaddr_in addr)
    {
        _addr=addr;
    }

private:
    struct sockaddr_in _addr;
};

#endif