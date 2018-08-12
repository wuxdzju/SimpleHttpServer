#ifndef BUFFER_H
#define BUFFER_H

#include <vector>
#include <unistd.h>
#include <assert.h>
#include <string>
#include <algorithm>

class Buffer
{
public:
    static const size_t kCheapPrepend=8;
    static const size_t kInitialSize=1024;

    Buffer()
            :_buff(kCheapPrepend+kInitialSize),
             _readerIndex(kCheapPrepend),
             _writerIndex(kCheapPrepend)
    {

    }

    void swap(Buffer& rhs)
    {
        _buff.swap(rhs._buff);
        std::swap(_readerIndex,rhs._readerIndex);
        std::swap(_writerIndex,rhs._writerIndex);
    }

    //缓冲区中待读取的字节数
    size_t readableBytes()const
    {
        return _writerIndex-_readerIndex;
    }

    //缓冲区中可写的字节数
    size_t writableBytes()const
    {
        return _buff.size()-_writerIndex;
    }

    //缓冲区中前面预留的字节数
    size_t prependableBytes()const
    {
        return _readerIndex;
    }

    //返回第一个可读的位置
    const char* peek() const
    {
        return begin()+_readerIndex;
    }

    //返回第一个可写的位置
    char* beginWrite()
    {
        return begin()+_writerIndex;
    }

    const char* beginWrite() const
    {
        return begin()+_writerIndex;
    }



    //一次读取n字节数据之后，更新_readerIndex的位置
    void retrieve(size_t len)
    {
        assert(len<=_readerIndex);
        _readerIndex+=len;
    }

    //一直读到end前一个位置，更新_readerIndex的位置
    void retrieveUntil(const char* end)
    {
        assert(peek()<=end);
        assert(end<=beginWrite());
        retrieve(end-peek());
    }

    //读取所有的数据后，更新_readerIndex和_writerIndex
    void retrieveAll()
    {
        _readerIndex=kCheapPrepend;
        _writerIndex=kCheapPrepend;
    }

    //将读到的数据以string的形式返回
    std::string retrieveAsString()
    {
        std::string str(peek(),readableBytes());
        retrieveAll();
        return str;
    }

    //更新_writeIndex的下标
    void hasWritten(size_t len)
    {
        _writerIndex+len;
    }

    //确保有len的空间可以写
    void ensureWritableBytes(size_t len)
    {
        if(writableBytes()<len)
        {
            makeSpace(len);
        }
        assert(writableBytes()>=len);
    }

    //将长度为len，首地址为data的数据追加到_buffer后面
    void append(const char* data,size_t len)
    {
        ensureWritableBytes(len);
        std::copy(data,data+len,beginWrite());
        hasWritten(len);
    }

    void append(const std::string& str)
    {
        append(&*str.begin(),str.size());
    }

    //在_readerIndex前面添加数据
    void prepend(const void* data,size_t len)
    {
        assert(len<=prependableBytes());
        _readerIndex-=len;
        const char *d= static_cast<const char*>(data);
        std::copy(d,d+len,begin()+_readerIndex);
    }

    void shrink(size_t reserve)
    {
        std::vector<char> buf(kCheapPrepend+readableBytes()+reserve);
        std::copy(peek(),peek()+readableBytes(),buf.begin()+kCheapPrepend);
        buf.swap(_buff);
    }

    //找到"\r\n出现的第一个位置"
    const char* findCRLF() const
    {
        const char* crlf = std::search(peek(), beginWrite(),D_CRLF,D_CRLF+2);
        return crlf == beginWrite() ? nullptr : crlf;
    }

    //直接读数据到buffer中
    ssize_t readFd(int fd,int *saveErrno);

private:
    char* begin()
    {
        return &*_buff.begin();
    }

    const char* begin() const
    {
        return &*_buff.begin();
    }

    void makeSpace(int len)
    {
        if(writableBytes()+prependableBytes()<len+kCheapPrepend)
        {
            _buff.resize(_writerIndex+len);
        }
        else
        {
            //把可读数据移到前面取，在后面腾出空间
            assert(kCheapPrepend<_readerIndex);
            size_t readable=readableBytes();
            std::copy(begin()+_readerIndex,begin()+_writerIndex,begin()+kCheapPrepend);
            _readerIndex=kCheapPrepend;
            _writerIndex=_readerIndex+readable;
            assert(readable==readableBytes());
        }
    }

private:
    std::vector<char> _buff;
    int _readerIndex;
    int _writerIndex;

    static const char D_CRLF[];
};

#endif