/*
 *Channel类主要负责事件的分发，每个Chanel只属于一个EventLoop，因此，每个Channel对象只属于一个IO线程。
 * 每个Channel对象自始至终只负责一个文件描述符fd的事件分发，但它不拥有这个fd，也不会在析构时关闭这个fd。
 * Channel会把不同的IO事件分发给不同的回调函数，如ReadCallBack，WriteCallBack等。
 * */

#ifndef CHANNEL_H
#define CHANNEL_H


#include <functional>
#include "base/noncopyable.h"
#include "base/TimeUnit.h"

class EventLoop;


class Channel:noncopyable
{
public:
    typedef std::function<void()> EventCallBack;
    typedef std::function<void(TimeUnit)> ReadEventCallBack;

    Channel(EventLoop *loop,int fd);

    ~Channel();

    void handEvent(TimeUnit recieveTime);

    void setReadCallBack(const ReadEventCallBack &rb)
    {
        _readCallBack=rb;
    }

    void setWriteCallBack(const EventCallBack &wb)
    {
        _writeCallBack=wb;
    }

    void setErrorCallBack(const EventCallBack &eb)
    {
        _errorCallBack=eb;
    }

    void setCloseCallBack(const EventCallBack& cb)
    {
        _closeCallBack=cb;
    }


    int fd() const
    {
        return _fd;
    }

    int events() const
    {
        return _events;
    }

    void setRevents(int revnts)
    {
        _revents=revnts;
    }

    bool isNoneEvent() const
    {
        return _events==D_NoneEvent;
    }

    void enableReading()
    {
        _events|=D_ReadEvent;
        update();
    }

    void enableWriting()
    {
        _events|=D_WriteEvent;
        update();
    }

    void disableWriting()
    {
        _events &=~D_WriteEvent;
        update();
    }

    bool isWriting() const
    {
        return _events & D_WriteEvent;
    }
    
    void disableAllEvent()
    {
        _events=D_NoneEvent;
        update();
    }

    //在Poller中使用
    int index() const
    {
        return _index;
    }
    void setIndex(int index)
    {
        _index=index;
    }

    EventLoop* ownerLoop() const
    {
        return _loop;
    }



private:
    void update();//该函数会调用Eventloop中的updateChannel函数，进而调用Poller中的updateChannel函数，最终的作用时更新Poller中该Channel对应的数据



    static const int D_NoneEvent;
    static const int D_ReadEvent;
    static const int D_WriteEvent;

    EventLoop* _loop;//Channel所属于的Eventloop
    const int _fd;//Channel所负责的文件描述符
    int _events;//需要监听的事件
    int _revents;//准备好了的事件
    int _index;//该Channel在Poller中对应数据结构的下标
    bool _eventhanding;//事件处理标志，表明该Channel上目前正有事件在处理中，该标志为true的话，不能执行析构

    ReadEventCallBack _readCallBack;
    EventCallBack _writeCallBack;
    EventCallBack _errorCallBack;
    EventCallBack _closeCallBack;
};

#endif