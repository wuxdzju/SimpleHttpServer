#include "Poller.h"
#include "Channel.h"
#include "EventLoop.h"
#include "base/TimeUnit.h"
#include <sys/poll.h>
#include <stdio.h>
#include <algorithm>

Poller::Poller(EventLoop *loop)
        :_ownerLoop(loop)
{

}

Poller::~Poller()
{

}

void Poller::assertInLoopThread()
{

    _ownerLoop->assertInLoopThread();
}

TimeUnit Poller::poll(int timeoutMs, ChannelList *activeChannels)
{
    int numEvents=::poll(&*_pollfds.begin(),_pollfds.size(),timeoutMs);
    TimeUnit retTime(TimeUnit::now());
    if(numEvents>0)
    {
        fillActiveChannels(numEvents,activeChannels);
    }
    else if(numEvents==0)
    {
        //什么也不做
    }
    else
    {//<0的情况,打印错误信息
        perror("Poller::poll:");
    }
    return retTime;
}


//fillActiveChannels函数遍历_pollfds数组，找出活动的fd，把它对应的Channel填入activeChannels中
void Poller::fillActiveChannels(int numEvents, ChannelList *activeChannels)
{
    for(PollFdList::const_iterator pfd=_pollfds.begin();pfd!=_pollfds.end();++pfd)
    {
        if(pfd->revents>0)
        {
            --numEvents;
            ChannelMap::const_iterator pcm=_channels.find(pfd->fd);//在_channels中找到对应的Channel
            assert(pcm!=_channels.end());
            Channel *channel=pcm->second;
            assert(channel->fd()==pfd->fd);
            channel->setRevents(pfd->revents);
            activeChannels->push_back(channel);
        }
    }
}

/*
 * 负责维护和更新_pollfds
 * 添加新的Channel（O（logN））
 * 更新已有的Channel（O（N））,因为Channel记住了自己在_pollfds的下标(index)
 * */
void Poller::updateChannel(Channel *channel) {
    assertInLoopThread();
    if(channel->index()<0){//一个新的Channel
        assert(_channels.find(channel->fd())==_channels.end());

        //新建一个pollfd
        struct pollfd pfd;
        pfd.fd=channel->fd();
        pfd.events= static_cast<short >(channel->events());
        pfd.revents=0;

        //建立pollfd和Channel之间的联系
        int idx= static_cast<int>(_pollfds.size());
        _pollfds.push_back(pfd);
        channel->setIndex(idx);
        _channels[pfd.fd]=channel;
    }
    else{
        //更新已经存在的pollfd
        assert(_channels.find(channel->fd())!=_channels.end());
        assert(_channels[channel->fd()]==channel);
        int idx=channel->index();
        assert(idx>=0 && idx<static_cast<int >(_pollfds.size()));
        struct pollfd &pfd=_pollfds[idx];
        assert(pfd.fd==channel->fd() || pfd.fd==-channel->fd()-1);//如果暂时不关心某个Channel的任何事件，就将pollfd的fd设置为-channel->fd()-1
        pfd.events= static_cast<short >(channel->events());
        pfd.revents=0;
        if(channel->isNoneEvent()){
            //忽略该Channel
            pfd.fd=-channel->fd()-1;//将fd设置为-channel->fd()-1，可以让::poll忽略此项
        }
    }
}

void Poller::removeChannel(Channel *channel)
{
    assertInLoopThread();

    assert(_channels.find(channel->fd())!=_channels.end());
    assert(_channels[channel->fd()]==channel);
    assert(channel->isNoneEvent());

    int idx=channel->index();
    assert(idx>=0 && idx< static_cast<int>(_pollfds.size()));
    const struct pollfd& pfd=_pollfds[idx];
    assert(pfd.fd == -channel->fd()-1 && pfd.events==channel->events());

    int n=_channels.erase(channel->fd());
    assert(n==1);

    if(static_cast<size_t >(idx)==_pollfds.size()-1)//要删除的元素时最后一个元素
    {
        _pollfds.pop_back();//直接删除
    }
    else
    {
        int channelEnd=_pollfds.back().fd;
        std::swap(_pollfds[idx],_pollfds[_pollfds.size()-1]);
        if(channelEnd<0)//channelEnd为当前channel的更改后的fd，其值等于(-实际的channel的fd-1）
        {
            channelEnd=-channelEnd-1;//这里的channelEnd为实际的channel的fd
        }
        _channels[channelEnd]->setIndex(idx);
        _pollfds.pop_back();
    }
}



