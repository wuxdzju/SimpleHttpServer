/*
 * Poller是对IO复用的封装，Poller是Eventloop的间接成员（指针类型），只供owner Eventloop在IO线程中调用，因此无需加锁。
 * 其生命周期与Eventloop相等；
 * Poller并不拥有Channel，Channel在析构之前必须unregister(EventLoop::removeChannel()),避免悬空指针
 * */

#ifndef POLLER_H
#define POLLER_H

#include "base/noncopyable.h"
#include "base/TimeUnit.h"
#include <vector>
#include <map>

class TimeUnit;
class Channel;
class EventLoop;
struct  pollfd;

class Poller:noncopyable
{
public:
    typedef std::vector<Channel*> ChannelList;

    Poller(EventLoop* loop);
    ~Poller();

    //整个Poller中最关键的函数，用于IO复用，将准备好的fd对应的Channel填入activeChannels中，并返回poll return的时刻
    //参数timeoutMs是传递给poll的参数，指定poll的超时值，-1将永远阻塞下去，直到某个事件发生，timeout为0时，poll将立刻返回
    TimeUnit poll(int timeoutMs,ChannelList* activeChannels);

    //改变对应Channel中感兴趣的事件
    //该函数必须有Eventloop调用
    void updateChannel(Channel* channel);

    void removeChannel(Channel* channel);

    void assertInLoopThread();

private:
    typedef std::vector<struct pollfd> PollFdList;
    typedef std::map<int,Channel*> ChannelMap;

    void fillActiveChannels(int numEvents,ChannelList* activeChannels);

    EventLoop* _ownerLoop;//所属的Eventloop
    PollFdList _pollfds;//pollfd数组（vector数组）
    ChannelMap _channels;//fd到Channel的映射
};

#endif