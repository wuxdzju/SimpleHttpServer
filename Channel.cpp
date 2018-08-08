#include "Channel.h"
#include "EventLoop.h"
#include <sys/poll.h>

const int Channel::D_NoneEvent=0;
const int Channel::D_ReadEvent=POLLIN | POLLPRI;
const int Channel::D_WriteEvent=POLLOUT;

Channel::Channel(EventLoop *loop, int fd)
        :_loop(loop),
         _fd(fd),
        _events(0),
        _revents(0),
        _index(-1),
         _eventhanding(false)
{

}

Channel::~Channel()
{
    assert(!_eventhanding);
}

void Channel::handEvent(TimeUnit recieveTime)
{
    _eventhanding=true;
    if((_revents & POLLHUP) && !(_revents && POLLIN))
    {
        if(_closeCallBack)
            _closeCallBack();
    }
    if(_revents & (POLLERR | POLLNVAL))
    {
        if(_errorCallBack)
            _errorCallBack();
    }
    if(_revents & POLLIN)
    {
        if(_readCallBack)
            _readCallBack(recieveTime);
    }
    if(_revents & POLLOUT)
    {
        if(_writeCallBack)
            _writeCallBack();
    }
    _eventhanding= false;
}

void Channel::update()
{
    _loop->updateChannel(this);
}