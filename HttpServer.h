#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "Server.h"
#include "base/TimeUnit.h"

class HttpRequest;
class HttpResponse;

class HttpServer:noncopyable
{
public:
    typedef std::function<void (const HttpRequest&,HttpResponse*)> HttpCallBack;

    HttpServer(EventLoop* loop, const InetAddr& listenAddr);

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
private:
    void OnConnection(const ConnectionPtr& conn);
    void OnMessage(const ConnectionPtr& conn, Buffer* buf, TimeUnit receiveTime);
    void OnRequest(const ConnectionPtr& conn, const HttpRequest&);

    Server _server;
    HttpCallBack _httpCallBack;
};

#endif