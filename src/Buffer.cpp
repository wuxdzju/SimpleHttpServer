#include "Buffer.h"
#include <sys/uio.h>

const char Buffer::D_CRLF[] = "\r\n";

ssize_t Buffer::readFd(int fd, int *saveErrno)
{
    char extrabuf[65536];
    struct iovec vec[2];
    const size_t writable=writableBytes();
    vec[0].iov_base=begin()+_writerIndex;
    vec[0].iov_len=writable;
    vec[1].iov_base=extrabuf;
    vec[1].iov_len= sizeof(extrabuf);

    const ssize_t  n=readv(fd,vec,2);
    if(n<0)
    {
        *saveErrno=errno;
    }
    else if(static_cast<size_t >(n)<=writable)
    {
        _writerIndex+=n;
    }
    else
    {
        _writerIndex=_buff.size();
        append(extrabuf,n-writable);
    }
    return n;
}