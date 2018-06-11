#include "http_tast.h"
#include <errno.h>
#include <sys/uio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdarg.h>
#include "util.h"
#include "epoll.h"
#include "myerr.h"

const int MAX_LINE=1024;


const char* ok_200_title = "OK";
const char* error_400_title = "Bad Request";
const char* error_400_form = "Your request has bad syntax or is inherently impossible to satisfy.\n";
const char* error_403_title = "Forbidden";
const char* error_403_form = "You do not have permission to get file from this server.\n";
const char* error_404_title = "Not Found";
const char* error_404_form = "The requested file was not found on this server.\n";
const char* error_500_title = "Internal Error";
const char* error_500_form = "There was an unusual problem serving the requested file.\n";
const char* root = "/var/www/html";

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
    init();
}

void http_tast::close_conn(){
    if(m_sockfd!=-1){
        removefd(m_epollfd,m_sockfd);
        m_sockfd=-1;
        m_ready_status=READY_NO;
        m_user_cnt--;
        //init();

    }
}

void http_tast::process(){
    HTTP_CODE read_code=NO_REQUEST;
    bool write_ret=false;
    switch (m_ready_status){
        case READY_READ:
            if(!read_buf()){//向用户读缓存中读数据发生错误
                close_conn();
                return;
            }
            read_code=process_read();
            if(read_code==NO_REQUEST){
                modifyfd(m_epollfd,m_sockfd,EPOLLIN);
                return;
            }
            write_ret=process_write(read_code);
            if(!write_ret){
                close_conn();
                return;
            }
            modifyfd(m_epollfd,m_sockfd,EPOLLOUT);//做好写的准备，向epoll中添加写事件
            break;
        case READY_WRITE:
            if(!write_buf()){
                close_conn();
                return;
            }
            break;
        default:
            break;
    }
}



void http_tast::init(){
    m_read_index=0;
    m_checked_index=0;
    m_line_start=0;


    m_check_state=CHECK_STATE_REQUESTLINE;
    m_method=GET;

    m_url=NULL;
    m_version=NULL;
    m_host=NULL;
    m_content_len=0;
    m_islinger=false;

    m_write_index=0;

    memset(m_read_buf,'\0',READ_BUF_SIZE);
    memset(m_filename,'\0',FILENAME_LEN);
    memset(m_write_buf,'\0',WRITE_BUF_SIZE);
}

bool http_tast::read_buf() {//循环读数据到读数据缓冲区，直到无数据可读，或对方关闭连接
    if(m_read_index>=READ_BUF_SIZE){
        return false;
    }

    int read_bytes=0;
    while(true){
        read_bytes=recv(m_sockfd,m_read_buf+m_read_index,READ_BUF_SIZE-m_read_index,0);
        if(read_bytes==-1){
            if(errno==EAGAIN || errno==EWOULDBLOCK){
                break;
            }
            return false;
        }
        else if(read_bytes==0){
            return false;
        }
        else{
            m_read_index+=read_bytes;
        }
    }
    return true;
}

HTTP_CODE http_tast::process_read(){
    LINE_STATUS line_status=LINE_OK;
    HTTP_CODE ret=NO_REQUEST;
    char *line=NULL;

    while(((m_check_state==CHECK_STATE_CONTENT) && (line_status==LINE_OK))
          || (line_status=parse_line())==LINE_OK){
        line=getline();
        m_line_start=m_checked_index;
        printf("receive one line:%s\n",line);

        switch (m_check_state){
            case CHECK_STATE_REQUESTLINE:
                ret=parse_request_line(line);
                if(ret==BAD_REQUEST){
                    return BAD_REQUEST;
                }
                break;
            case CHECK_STATE_HEADER:
                ret=parse_headers(line);
                if(ret==BAD_REQUEST){
                    return BAD_REQUEST;
                }
                else if(ret==GET_REQUEST){
                    return do_request();
                }
                break;
            case CHECK_STATE_CONTENT:
                ret=parse_content(line);
                if(ret==GET_REQUEST){
                    return do_request();
                }
                line_status=LINE_OPEN;
                break;
            default:
                return INTERNAL_ERROR;

        }
    }
    if(line_status==LINE_BAD){
        return BAD_REQUEST;
    }
    return NO_REQUEST;
}

//从状态机，用于解析一行的内容
HTTP_CODE http_tast::parse_request_line(char *line){
    m_url=strpbrk(line," \t");
    if(!m_url){
        return BAD_REQUEST;
    }
    *m_url++='\0';

    char *method=line;
    if(strcasecmp(method,"GET")==0){
        m_method=GET;
    }
    else{
        return BAD_REQUEST;
    }

    m_url+=strspn(m_url," \t");
    m_version=strpbrk(m_url," \t");
    *m_version++='\0';
    m_version+=strspn(m_version," \t");
    if(strcasecmp(m_version,"HTTP/1.1")!=0){
        return BAD_REQUEST;
    }

    if(strncasecmp(m_url,"http://",7)==0){
        m_url+=7;
        m_url=strchr(m_url,'/');
    }

    if(!m_url || m_url[0]!='/'){
        return BAD_REQUEST;
    }

    m_check_state=CHECK_STATE_HEADER;
    return NO_REQUEST;
}

//解析请求行，获得请求方法，目标URL，以及HTTP版本号
HTTP_CODE http_tast::parse_headers(char *line){
    if(line[0]=='\0'){
        //如果HTTP请求有消息体，则还需读取m_content_len字节的消息体
        if(m_content_len!=0){
            m_check_state=CHECK_STATE_CONTENT;
            return NO_REQUEST;
        }
        //没有的话，则说明已经得到了一个HTTP请求
        return GET_REQUEST;
    }

        //处理Connection头部字段
    else if(strncasecmp(line,"Connection:",11)==0){
        line+=11;
        line+=strspn(line," \t");
        if(strcasecmp(line,"keep-alive")==0){
            m_islinger=true;
        }
    }

        //处理Content-Length头部字段
    else if(strncasecmp(line,"Content-Length:",15)==0){
        line+=15;
        line+=strspn(line," \t");
        m_content_len=atol(line);
    }

        //处理host头部字段
    else if(strncasecmp(line,"host:",5)==0){
        line+=5;
        line+=strspn(line," \t");
        m_host=line;
    }
    else{
        printf("unknow header %s\n",line);
    }
    return NO_REQUEST;
}

//并没有真正解析消息体，只是判断它是否被完整地读入了
HTTP_CODE http_tast::parse_content(char *line){
    if(m_read_index>=(m_content_len+m_checked_index)){
        line[m_content_len]='\0';
        return GET_REQUEST;
    }
    return NO_REQUEST;
}

http_tast::LINE_STATUS http_tast::parse_line(){
    char tmp;
    for(;m_checked_index<m_read_index;m_checked_index++){
        tmp=m_read_buf[m_checked_index];
        if(tmp=='\r'){//当前检查到回车键
            if((m_checked_index+1)==m_read_index){
                return LINE_OPEN;
            }
            else if(m_read_buf[m_checked_index+1]=='\n'){
                m_read_buf[m_checked_index++]='\0';
                m_read_buf[m_checked_index++]='\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
        else if(tmp=='\n'){
            if((m_checked_index>1)&&(m_read_buf[m_checked_index-1]=='\r')){
                m_read_buf[m_checked_index-1]='\0';
                m_read_buf[m_checked_index++]='\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
    }
    return LINE_OPEN;
}

HTTP_CODE http_tast::do_request(){
    strcpy(m_filename,root);
    int len=strlen(root);
    strncpy(m_filename+len,m_url,FILENAME_LEN-len-1);
    if(stat(m_filename,&m_file_stat)!=0){
        return NO_RESOURCE;
    }
    if(!(m_file_stat.st_mode & S_IROTH)){
        return FORBIDDEN_REQUEST;
    }
    if(S_ISDIR(m_file_stat.st_mode)){
        return BAD_REQUEST;
    }

    int fd=open(m_filename,O_RDONLY);
    m_file_addr=(char*)mmap(0,m_file_stat.st_size,PROT_READ,MAP_PRIVATE,fd,0);
    close(fd);
    return FILE_REQUEST;
}


bool http_tast::write_buf(){//返回true则不关闭连接，返回false的话则关闭连接
    int tmp=0;
    int byte_have_send=0;
    //int bytes_to_send=m_write_index;
    if(m_write_index==0){//没有数据可发送，监听读事件
        modifyfd(m_epollfd,m_sockfd,EPOLLIN);
        init();
        return true;
    }
    while(true){
        tmp=writev(m_sockfd,m_iv,m_iv_cnt);
        if(tmp<=-1){
            //如果TCP没有缓冲空间，则等待下一轮EPOLLOUT事件。虽然在此期间服务器无法立即接收到同一客户的下一个请求，但这可保证连接的完整性。
            if(errno==EAGAIN){
                modifyfd(m_epollfd,m_sockfd,EPOLLOUT);
                return true;
            }
            unmap();
            return false;
        }
        byte_have_send+=tmp;
        if(byte_have_send>=m_write_index){
            //根据是否是长连接来确定是否立即关闭连接
            unmap();
            if(m_islinger){
                init();
                modifyfd(m_epollfd,m_sockfd,EPOLLIN);
                return true;
            }
            else{
                modifyfd(m_epollfd,m_sockfd,EPOLLIN);
                return false;
            }
        }
    }
}

bool http_tast::process_write(HTTP_CODE stat_code){
    switch (stat_code){
        case BAD_REQUEST:
            add_status_line(400,error_400_title);
            add_headers(strlen(error_400_form));
            if(!add_content(error_400_form)){
                return false;
            }
            break;
        case FORBIDDEN_REQUEST:
            add_status_line(403,error_403_title);
            add_headers(strlen(error_403_form));
            if(!add_content(error_403_form)){
                return false;
            }
            break;
        case NO_RESOURCE:
            add_status_line(404,error_404_title);
            add_headers(strlen(error_404_form));
            if(!add_content(error_404_form)){
                return false;
            }
            break;
        case INTERNAL_ERROR:
            add_status_line(500,error_500_title);
            add_headers(strlen(error_500_form));
            if(!add_content(error_500_form)){
                return false;
            }
            break;
        case FILE_REQUEST:
            add_status_line(200,ok_200_title);
            if(m_file_stat.st_size!=0){
                add_headers(m_file_stat.st_size);
                m_iv[0].iov_base=m_write_buf;
                m_iv[0].iov_len=m_write_index;
                m_iv[1].iov_base=m_file_addr;
                m_iv[1].iov_len=m_file_stat.st_size;
                m_iv_cnt=2;
                return true;
            }
            else{
                const char *ok_string="<html><body></body></html>";
                add_headers(strlen(ok_string));
                if(!add_content(ok_string)){
                    return false;
                }
            }
        default:
            return false;
    }
    m_iv[0].iov_base=m_write_buf;
    m_iv[0].iov_len=m_write_index;
    m_iv_cnt=1;
    return true;
}

void http_tast::unmap(){
    if(m_file_addr){
        munmap(m_file_addr,m_file_stat.st_size);
        m_file_addr=NULL;
    }
}

bool http_tast::add_response(const char *format,...){//往缓冲区中写待发送的数据
    if(m_write_index>=WRITE_BUF_SIZE){
        return false;
    }

    va_list arg_list;
    va_start(arg_list,format);
    int len=vsnprintf(m_write_buf+m_write_index,WRITE_BUF_SIZE-m_write_index-1,format,arg_list);
    if(len>WRITE_BUF_SIZE-m_write_index-1){
        return false;
    }
    m_write_index+=len;
    va_end(arg_list);
    return true;
}

bool http_tast::add_status_line(int status,const char* title){
    return add_response("%s %d %s\r\n","HTTP/1.1",status,title);
}

bool http_tast::add_headers(int content_len){
    add_content_len(content_len);
    add_linger();
    add_blank_line();
}

bool http_tast::add_content_len(int content_len){
    return add_response("Content-length:%d\r\n",content_len);
}

bool http_tast::add_linger(){
    return add_response("Connection:%s\r\n",(m_islinger?"keep-alive":"close"));
}

bool http_tast::add_blank_line(){
    return add_response("%s","\r\n");
}

bool http_tast::add_content(const char *content){
    return add_response("%s",content);
}

