#include "HttpServer.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "EventLoop.h"
#include "Server.h"

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

void defaultHttpCallBack(const HttpRequest&, HttpResponse* response)
{
    response->setStatusCode(HttpResponse::D_404_NOT_FOUND);
    response->setStatusMessage("Not Found");
    response->setCloseConnection(true);
}

HttpServer::HttpServer(EventLoop *loop,
                       const InetAddr &listenAddr)
        : _server(loop,listenAddr),
          _httpCallBack(defaultHttpCallBack)
{
    _server.setConnectionCallBack(
            std::bind(&HttpServer::OnConnection,this,_1));
    _server.setMessageCallBack(
            std::bind(&HttpServer::OnMessage,this,_1,_2,_3));
}

HttpServer::~HttpServer()
{

}

void HttpServer::start()
{
    _server.start();
}

void HttpServer::OnConnection(const ConnectionPtr &conn)
{
    if(conn->Connected())
    {
        conn->setHttpTask(HttpTask());
    }
}

void HttpServer::OnMessage(const ConnectionPtr &conn,
                           Buffer *buf,
                           TimeUnit receiveTime)
{
    HttpTask* httpTask = conn->getHttpTask();
    if(!httpTask->parseRequest(buf,receiveTime))
    {
        conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
        conn->shutdown();
    }

    if(httpTask->isCheckDone())
    {
        OnRequest(conn,httpTask->getRequest());
        httpTask->reset();
    }
}

void HttpServer::OnRequest(const ConnectionPtr& conn,
                           const HttpRequest& request)
{
    const std::string& connection = request.getHeader("Connection");
    bool close = connection == "close" ||
            (request.getVersion() == HttpRequest::D_HTTP_10 && connection != "Keep-Alive");
    HttpResponse response(close);
    _httpCallBack(request, &response);
    Buffer buf;
    response.appendToBuffer(&buf);
}