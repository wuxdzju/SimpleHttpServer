#include "http_tast.h"

int http_tast::m_epollfd=-1;
int http_tast::m_user_cnt=0;

void http_tast::init(int sockfd,const sockaddr_in &addr){

    m_sockfd=sockfd;
    m_address=addr;
    m_ready_status=READY_NO;
    //以下两行为避免TIME_WAIT状态，用于调试
    int op=1;
    setsockopt(m_sockfd,SOL_SOCKET,SO_REUSEADDR,&op, sizeof(op));
    addfd(m_epollfd,sockfd, true);
    m_user_cnt++;
    m_http_parse.init(m_epollfd,sockfd);
}

void http_tast::close_conn(){
    if(m_sockfd!=-1){
        removefd(m_epollfd,m_sockfd);
        m_sockfd=-1;
        m_ready_status=READY_NO;
        m_http_parse.init(m_epollfd,m_sockfd);
        m_user_cnt--;
    }
}

void http_tast::process(){
    HTTP_CODE read_code=NO_REQUEST;
    bool write_ret=false;
    switch (m_ready_status){
        case READY_READ:
            if(!m_http_parse.read_buf()){//向用户读缓存中读数据发生错误
                close_conn();
                return;
            }
            read_code=m_http_parse.process_read();
            if(read_code==NO_REQUEST){
                modifyfd(m_epollfd,m_sockfd,EPOLLIN);
                return;
            }
            write_ret=m_http_parse.process_write(read_code);
            if(!write_ret){
                close_conn();
                return;
            }
            modifyfd(m_epollfd,m_sockfd,EPOLLOUT);//做好写的准备，向epoll中添加写事件
            break;
        case READY_WRITE:
            if(!m_http_parse.write_buf()){
                close_conn();
                return;
            }
            break;
        default:
            break;
    }
}