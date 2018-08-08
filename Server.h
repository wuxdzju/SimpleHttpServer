#ifndef SERVER_H
#define SERVER_H

#include "base/noncopyable.h"
#include "base/CallBack.h"
#include "Connection.h"

#include <map>
#include <string>
#include <memory>

class Acceptor;

class Server:noncopyable
{
public:
    Server(EventLoop* loop,const InetAddr& listenAddr);

    ~Server();

    //线程安全的
    void start();

    //不是线程安全的
    void setConnectionCallBack(const ConnectionCallBack& cb)
    {
        _connectionCallBack=cb;
    }

    //不是线程安全的
    void setMessageCallBack(const MessageCallBack& cb)
    {
        _messageCallBack=cb;
    }

private:
    //不是线程安全的，但是在loop中
    void newConnection(int sockfd,const InetAddr& peerAddr);
    void removeConnection(const ConnectionPtr& conn);

    typedef std::map<std::string,ConnectionPtr> ConnectionMap;

    EventLoop* _loop;
    const std::string _name;
    std::unique_ptr<Acceptor> _acceptor;
    ConnectionCallBack _connectionCallBack;
    MessageCallBack _messageCallBack;
    bool _started;
    int _nextConnId;
    ConnectionMap _connections;
};

#endif