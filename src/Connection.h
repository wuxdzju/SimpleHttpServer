#ifndef CONNECTION_H
#define CONNECTION_H

#include <memory>
#include <string>
#include "base/noncopyable.h"
#include "base/CallBack.h"
#include "base/TimeUnit.h"
#include "InetAddr.h"
#include "Buffer.h"
#include "HttpTask.h"

class EventLoop;
class Socket;
class Channel;



class Connection:noncopyable,
                 public std::enable_shared_from_this<Connection>
{
public:
    Connection(EventLoop *loop,
               const std::string name,
               int socket,
               const InetAddr& localAddr,
               const InetAddr& peerAddr
    );

    ~Connection();

    EventLoop* getLoop() const {return _loop;}

    const std::string& getName() const { return _name; }

    const InetAddr& getLocalAddr() { return _localAddr; }

    const InetAddr& getPeerAddr() { return  _peerAddr; }

    TimeUnit getLastActiveTime() const { return _lastActiveTime; }

    void setLastActiveTime(const TimeUnit& lastActiveTime)
    {
        _lastActiveTime = lastActiveTime;
    }

    bool Connected() const { return  _connState==D_CONNECTED; }

    void setConnectionCallback(const ConnectionCallBack& cb)
    {
        _connectionCallback = cb;
    }

    void setMessageCallback(const MessageCallBack& cb)
    {
        _messageCallback = cb;
    }

    void setCloseCallback(const CloseCallBack& cb)
    {
        _closeCallback=cb;
    }

    void setHttpTask(HttpTask httpTask)
    {
        _httpTask = httpTask;
    }

    HttpTask* getHttpTask()
    {
        return &_httpTask;
    }

    void setWeakConnectionListPosition(WeakConnectionList::iterator it)
    {
        _weakConnectionListPosiion = it;
    }

    WeakConnectionList::iterator getWeakConnectionListPosition() const
    {
        return _weakConnectionListPosiion;
    }

    //在Server中调用，当Server接受一个新连接时调用，并且只调用一次
    void ConnectEstablished();

    //析构前调用的最后一个函数,用于通知用户连接已断开
    void connectionDestroyed();

    //发送数据，线程安全的
    void send(const std::string& message);
    void send(Buffer* buf);

    //线程安全的
    void shutdown();

    //主动关闭连接，线程安全的
    void forceClose();
    void forceCloseWithDelay(int seconds);

private:
    enum ConnectionState
    {
        D_CONNECTING,
        D_CONNECTED,
        D_DISCONNECTED,
        D_DISCONNECTING
    };

    void setConnState(ConnectionState s){
        _connState = s;
    }

    void handRead(TimeUnit recieveTime);
    void handWrite();
    void handError();
    void handClose();

    void sendInLoop(const std::string& message);
    void shutdownInLoop();
    void forceCloseInLoop();

    //bool canDeleted() const ;


    EventLoop *_loop;
    std::string _name;
    ConnectionState _connState;

    std::unique_ptr<Socket> _socket;
    std::unique_ptr<Channel> _channel;
    InetAddr _localAddr;
    InetAddr _peerAddr;

    TimeUnit _lastActiveTime;

    Buffer _inputBuffer;
    Buffer _outputBuffer;

    HttpTask _httpTask;

    WeakConnectionList::iterator _weakConnectionListPosiion;

    ConnectionCallBack _connectionCallback;//连接刚建立好之后的调用的回调函数
    MessageCallBack _messageCallback;//收到数据（即read到数据）之后调用的回调函数
    CloseCallBack _closeCallback;//当read到文件结尾标志时调用
};

#endif