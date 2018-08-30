#include "InetAddr.h"
#include <string.h>
#include <netinet/in.h>
#include "SocketHand.h"

using namespace SocketHand;

InetAddr::InetAddr(uint16_t port)
{
    bzero(&_addr, sizeof(_addr));
    _addr.sin_family=AF_INET;
    _addr.sin_port=hostToNet16(port);
    _addr.sin_addr.s_addr=hostToNet32(INADDR_ANY);
}

InetAddr::InetAddr(const std::string &ip, uint16_t port)
{
    bzero(&_addr, sizeof(_addr));
    hostPortToSockAddr(ip.c_str(),port,&_addr);
}

std::string InetAddr::toHostPort() const
{
    char buf[32];
    sockAddrToHostPort(&_addr,buf, sizeof(buf));
    return buf;
}