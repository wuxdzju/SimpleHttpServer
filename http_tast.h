#ifndef HTTP_TAST_H
#define HTTP_TAST_H

#include "http_parse.h"
#include <sys/socket.h>


class http_tast{
public:
    http_tast():m_ready_status(READY_NO){}
    ~http_tast(){}

public:
    void init(int sockfd,const sockaddr_in &addr);
    void close_conn();
    void process();

public:
    static int m_epollfd;
    static int m_user_cnt;

public:
    READY_STATUS m_ready_status;

private:
    int m_sockfd;
    sockaddr_in m_address;

    http_parse m_http_parse;
};

#endif