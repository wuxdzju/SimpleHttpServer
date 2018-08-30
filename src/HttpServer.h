#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "Server.h"
#include "base/TimeUnit.h"

#include <list>

class HttpRequest;
class HttpResponse;

class HttpServer:noncopyable
{
public:
    typedef std::function<void (const HttpRequest&,HttpResponse*)> HttpCallBack;



    HttpServer(EventLoop* loop, const InetAddr& listenAddr, int threadNum, int idleSeconds = 8);

    ~HttpServer();

    EventLoop* getLoop() const
    {
        return _server.getLoop();
    }

    void setHttpCallBack(const HttpCallBack& cb)
    {
        _httpCallBack = cb;
    }

    void start();

    void OnTimer();

    void addConnList(const ConnectionPtr& conn);
    void eraseConnList(const ConnectionPtr& conn);
    void modifyConnList(const ConnectionPtr& conn);

private:
    void OnConnection(const ConnectionPtr& conn);
    void OnMessage(const ConnectionPtr& conn, Buffer* buf, TimeUnit receiveTime);
    void OnRequest(const ConnectionPtr& conn, const HttpRequest&);

    void addConnListInLoop(const ConnectionPtr& conn);
    void eraseConnListInLoop(const ConnectionPtr& conn);
    void modifyConnListInLoop(const ConnectionPtr& conn);

    Server _server;
    HttpCallBack _httpCallBack;

    int _idleSeconds;
    WeakConnectionList _connectionList;
};

#endif