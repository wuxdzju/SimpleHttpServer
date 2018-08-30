#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "base/noncopyable.h"
#include "Channel.h"
#include "Socket.h"
#include "InetAddr.h"
#include "SocketHand.h"



#include <functional>

class EventLoop;


class Acceptor:noncopyable
{
public:
    typedef std::function<void(int,const InetAddr&)> NewConnCallBack;

    Acceptor(EventLoop* loop,const InetAddr& listenAddr);

    void listen();

    bool isListening()const
    {
        return _listening;
    }

    void setNewConnCallBack(const NewConnCallBack& cb)
    {
        _newConnCallBack=cb;
    }


private:
    void handRead();

    EventLoop* _loop;
    Socket _acceptSocket;
    Channel _acceptChannel;
    NewConnCallBack _newConnCallBack;
    bool _listening;
};

#endif