#ifndef HTTP_TAST_H
#define HTTP_TAST_H


#include <sys/socket.h>
#include <sys/uio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdarg.h>
#include "util.h"
#include "epoll.h"
#include "myerr.h"


class http_tast{
public:
    static const int FILENAME_LEN=200;
    static const int READ_BUF_SIZE=2048;
    static const int WRITE_BUF_SIZE=2048;
    enum METHOD { GET = 0, POST, HEAD, PUT, DELETE, TRACE, OPTIONS, CONNECT, PATCH };
    enum CHECK_STATE { CHECK_STATE_REQUESTLINE = 0, CHECK_STATE_HEADER, CHECK_STATE_CONTENT };
    enum LINE_STATUS { LINE_OK = 0, LINE_BAD, LINE_OPEN };

public:
    http_tast():m_ready_status(READY_NO){}
    ~http_tast(){}

public:
    void init(int sockfd,const sockaddr_in &addr);
    void close_conn();
    void process();

private:
    void init();
    bool read_buf();
    HTTP_CODE process_read();
    bool write_buf();
    bool process_write(HTTP_CODE);

    HTTP_CODE parse_request_line(char *buf);
    HTTP_CODE parse_headers(char *buf);
    HTTP_CODE parse_content(char *buf);
    char* getline(){ return m_read_buf+m_line_start;}
    LINE_STATUS parse_line();
    HTTP_CODE do_request();

    void unmap();
    bool add_response(const char *format,...);
    bool add_status_line(int status,const char* title);
    bool add_headers(int content_len);
    bool add_content_len(int content_len);
    bool add_linger();
    bool add_blank_line();
    bool add_content(const char *content);

public:
    static int m_epollfd;
    static int m_user_cnt;

public:
    READY_STATUS m_ready_status;

private:
    int m_sockfd;
    sockaddr_in m_address;

    char m_read_buf[READ_BUF_SIZE];//读缓冲区
    char m_read_index;//标识读缓冲区中已经读入的最后一个字节的下一个位置
    char m_checked_index;//当前正在解析的位置
    int m_line_start;//当前正在解析行的起始位置

    CHECK_STATE m_check_state;//主状态机当前所处的状态
    METHOD m_method;//请求方法


    char m_filename[FILENAME_LEN];//目标文件名（root+m_url)
    char *m_url;//url
    char *m_version;//版本号
    char *m_host;//主机名
    int m_content_len;//消息体的长度
    bool m_islinger;//是否保持长连接

    char m_write_buf[WRITE_BUF_SIZE];//写缓冲区
    int m_write_index;//写缓冲区待发的字节数

    char* m_file_addr;//客户请求的目标文件被mmap到的内存中的位置
    struct stat m_file_stat;//目标文件的状态
    struct iovec m_iv[2];//我们使用writev(集中写），需要的数据结构
    int m_iv_cnt;
};

#endif