#ifndef HTTPTASK_H
#define HTTPTASK_H

#include "HttpRequest.h"

class HttpTask
{
public:
    enum HttpRequestCheckState
    {
        D_CHECK_REQUESTLINE = 0,
        D_CHECK_HEADER,
        D_CHECK_CONTENT,
        D_CHECK_DONE
    };

    HttpTask()
            : _checkState(D_CHECK_REQUESTLINE)
    {

    }

    const HttpRequest& getRequest() const
    {
        return _request;
    }

    HttpRequest& getRequest()
    {
        return _request;
    }

    void reset()
    {
        _checkState = D_CHECK_REQUESTLINE;
        HttpRequest dummy;
        _request.swap(dummy);
    }

    bool isCheckDone() const
    {
        return _checkState == D_CHECK_DONE;
    }

    bool parseRequest(Buffer* buffer, TimeUnit receiveTime);

private:
    bool processRequestLine(const char* begin,const char* end);

    HttpRequest _request;
    HttpRequestCheckState _checkState;
};

#endif