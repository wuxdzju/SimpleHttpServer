#ifndef SOCKETOHAND_H
#define SOCKETOHAND_H

#include <sys/socket.h>
#include <fcntl.h>
#include <string.h>//bzero
#include <unistd.h>
#include <stdint.h>
#include <netinet/in.h>


namespace SocketHand
{
    typedef struct sockaddr SA;

    const SA* sockaddr_cast(const struct sockaddr_in* addr);

    SA* sockaddr_cast(struct sockaddr_in* addr);


    void setNonBlockAndCloseOnExec(int sockfd);

    int createNonBlockingOrDie();

    int createBlockingOrDie();

    void bindOrDie(int sockfd,const struct sockaddr_in& addr);

    void listenOrDie(int sockfd);

    int acceptConn(int sockfd, struct sockaddr_in* addr);

    void closeFd(int sockfd);

    /*
     * 网络字节序的转换
     * */

    uint16_t hostToNet16(uint16_t host16);
    uint16_t netToHost16(uint16_t net16);
    uint32_t hostToNet32(uint32_t host32);
    uint32_t netToHost32(uint32_t net32);


    /*
     * host:port和sockaddr之间的转换函数
     * */
    void hostPortToSockAddr(const char *ip,uint16_t port, struct sockaddr_in *addr);

    void sockAddrToHostPort(const struct sockaddr_in *addr,char* buf,size_t size);

    //通过sockfd获取本地地址
    struct sockaddr_in getLocalAddr(int sockfd);


}



#endif