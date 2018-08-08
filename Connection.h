#ifndef CONNECTION_H
#define CONNECTION_H

#include <memory>
#include <string>
#include "base/noncopyable.h"
#include "base/CallBack.h"
#include "base/TimeUnit.h"
#include "InetAddr.h"
#include "Buffer.h"

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
               const InetAddr& peerAddr);

    ~Connection();

    EventLoop* getLoop() const {return _loop;}

    const std::string& getName() const { return _name; }

    const InetAddr& getLocalAddr() { return _localAddr; }

    const InetAddr& getPeerAddr() { return  _peerAddr; }

    bool Connected() const { return  _connState==D_CONNECTED; }

    void setConnectionCallback(const ConnectionCallBack& cb)
    {
        _connectionCallback=cb;
    }

    void setMessageCallback(const MessageCallBack& cb)
    {
        _messageCallback=cb;
    }

    void setCloseCallback(const CloseCallBack& cb)
    {
        _closeCallback=cb;
    }

    //在Server中调用，当Server接受一个新连接时调用，并且只调用一次
    void ConnectEstablished();

    //析构前调用的最后一个函数,用于通知用户连接已断开
    void connectionDestroyed();

private:
    enum ConnectionState
    {
        D_CONNECTING,
        D_CONNECTED,
        D_DISCONNECTED
    };

    void setConnState(ConnectionState s){
        _connState=s;
    }

    void handRead(TimeUnit recieveTime);
    void handWrite();
    void handError();
    void handClose();

    EventLoop *_loop;
    std::string _name;
    ConnectionState _connState;

    std::unique_ptr<Socket> _socket;
    std::unique_ptr<Channel> _channel;
    InetAddr _localAddr;
    InetAddr _peerAddr;

    Buffer _inputBuffer;

    ConnectionCallBack _connectionCallback;//连接刚建立好之后的调用的回调函数
    MessageCallBack _messageCallback;//收到数据（即read到数据）之后调用的回调函数
    CloseCallBack _closeCallback;//当read到文件结尾标志时调用
};

#endif