#include "HttpTask.h"
#include "Buffer.h"

#include <algorithm>

bool HttpTask::processRequestLine(const char *begin, const char *end)
{
    bool succeed = false;
    const char* start = begin;
    const char* space = std::find(start, end, ' ');
    if(start != end && _request.setMethod(start, space))
    {
        start = space + 1;
        space =std::find(start, end, ' ');
        if(space != end)
        {
            const char* question =std::find(start, space, '?');
            if(question != space)
            {
                _request.setUrl(start, question);
                _request.setQuery(question,end);
            }
            else
            {
                _request.setUrl(start, space);
            }

            start =space + 1;
            succeed = (end - start == 8 && std::equal(start, end-1, "HTTP/1."));
            if(succeed)
            {
               if(*(end-1) == '1')
               {
                   _request.setVersion(HttpRequest::D_HTTP_11);
               }
                else if(*(end - 1) == '0')
               {
                   _request.setVersion(HttpRequest::D_HTTP_10);
               }
                else
               {
                   succeed = false;
               }
            }

        }
    }
    return  succeed;
}

bool HttpTask::parseRequest(Buffer *buffer, TimeUnit receiveTime)
{
    bool ok = true;
    bool hasMore = true;
    while(hasMore)
    {
        if(_checkState == D_CHECK_REQUESTLINE)
        {
            const char* crlf = buffer->findCRLF();
            if(crlf)
            {
                ok = processRequestLine(buffer->peek(),crlf);
                if(ok)
                {
                    _request.setReceiveTime(receiveTime);
                    buffer->retrieveUntil(crlf+2);
                    _checkState = D_CHECK_HEADER;
                }
                else
                {
                    hasMore = false;
                }
            }
            else
            {
                hasMore = false;
            }
        }
        else if (_checkState == D_CHECK_HEADER)
        {
            const char* crlf = buffer->findCRLF();
            if (crlf)
            {
                const char* colon = std::find(buffer->peek(), crlf, ':');
                if (colon != crlf)
                {
                    _request.addHeaders(buffer->peek(), colon, crlf);
                }
                else
                {
                    //空行：只包含crlf,代表header的结束
                    _checkState = D_CHECK_DONE;
                    hasMore = false;
                }
                buffer->retrieveUntil(crlf + 2);
            }
            else
            {
                hasMore = false;
            }
        }
        else if (_checkState == D_CHECK_CONTENT)
        {
            //todo
        }
    }
    return  ok;
}