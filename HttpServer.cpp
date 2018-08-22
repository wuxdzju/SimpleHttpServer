#include "HttpServer.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "EventLoop.h"
#include "Server.h"



using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

extern const int timeOutSeconds = 8;//超时时间8s，连接8秒没input的话，则关闭连接

extern EventLoop* g_loop;
extern HttpServer* myserver;

void defaultHttpCallBack(const HttpRequest&, HttpResponse* response)
{
    response->setStatusCode(HttpResponse::D_404_NOT_FOUND);
    response->setStatusMessage("Not Found");
    response->setCloseConnection(true);
}

HttpServer::HttpServer(EventLoop *loop,
                       const InetAddr &listenAddr,
                       int idleSeconds)
        : _server(loop,listenAddr),
          _httpCallBack(defaultHttpCallBack),
          _idleSeconds(idleSeconds)
{
    _server.setConnectionCallBack(
            std::bind(&HttpServer::OnConnection,this,_1));
    _server.setMessageCallBack(
            std::bind(&HttpServer::OnMessage,this,_1,_2,_3));
    loop->runEvery(1,std::bind(&HttpServer::OnTimer,this));
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

        conn->setLastActiveTime(TimeUnit::now());
        _connectionList.push_back(conn);
        conn->setWeakConnectionListPosition(--_connectionList.end());
        printf("OnConnection(): tid=%d new connection [%s] from %s\n",
               CurrentThread::tid(),
               conn->getName().c_str(),
               conn->getPeerAddr().toHostPort().c_str());

//        EventLoop* ioLoop = conn->getLoop();
//        ioLoop->runInLoopThread(
//            std::bind(&Connection::forceCloseWithDelay,conn,timeOutSeconds));
    }
    else
    {
        _connectionList.erase(conn->getWeakConnectionListPosition());
    }
}

void HttpServer::OnMessage(const ConnectionPtr &conn,
                           Buffer *buf,
                           TimeUnit receiveTime)
{
//    EventLoop* ioLoop = conn->getLoop();
//    ioLoop->runInLoopThread(
//            std::bind(&Connection::setLastActiveTime,conn,receiveTime));
//    ioLoop->runInLoopThread(
//            std::bind(&Connection::forceCloseWithDelay,conn,timeOutSeconds));

    conn->setLastActiveTime(receiveTime);
    _connectionList.splice(_connectionList.end(),_connectionList,conn->getWeakConnectionListPosition());
    assert(--_connectionList.end() == conn->getWeakConnectionListPosition());

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
    conn->send(&buf);
    if(response.closeConnection())
    {
        conn->shutdown();
    }
}

void HttpServer::OnTimer()
{
    TimeUnit now = TimeUnit::now();
    for(WeakConnectionList::iterator it = _connectionList.begin();
            it != _connectionList.end();)
    {
        ConnectionPtr conn = it->lock();
        if(conn)
        {
            double time = timeDifference(now,conn->getLastActiveTime());
            if(time / TimeUnit::kMicroSecondsPerSecond > _idleSeconds)
            {
                if(conn->Connected())
                {
                    conn->shutdown();
                    it++;
                    //conn->forceClose();
                }

            }
            else if(time < 0)
            {
                conn->setLastActiveTime(now);
            }
            else
            {
                break;
            }
        }
        else
        {
            it = _connectionList.erase(it);
        }
    }
}

void HttpServer::addConnList(const ConnectionPtr &conn)
{
    EventLoop* loop = _server.getLoop();
    loop->runInLoopThread(std::bind(&HttpServer::addConnListInLoop,,))
}

void HttpServer::addConnListInLoop(const ConnectionPtr &conn)
{

    _connectionList.splice(_connectionList.end(),_connectionList,conn->getWeakConnectionListPosition());
    assert(--_connectionList.end() == conn->getWeakConnectionListPosition());
}
