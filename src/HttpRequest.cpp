#include "HttpRequest.h"

bool HttpRequest::setMethod(const char *start, const char *end)
{
    assert(_method == D_INVALID);
    std::string method(start,end);
    std::transform(method.begin(), method.end(), method.begin(), toupper);
    if(method == "GET")
    {
        _method = D_GET;
    }
    else if(method == "POST")
    {
        _method = D_POST;
    }
    else if (method == "HEAD")
    {
        _method = D_HEAD;
    }
    else if(method == "PUT")
    {
        _method = D_PUT;
    }
    else if(method == "DELETE")
    {
        _method = D_DELETE;
    }
    else if(method == "TRACE")
    {
        _method = D_TRACE;
    }
    else if(method == "OPTIONS")
    {
        _method = D_OPTIONS;
    }
    else if(method == "CONNECT")
    {
        _method = D_CONNECT;
    }
    else if(method == "PATCH")
    {
        _method = D_PATCH;
    }
    else
    {
        _method = D_INVALID;
    }

    return _method;
}

void HttpRequest::addHeaders(const char *start, const char *colon, const char *end)
{
    std::string key(start, colon);
    ++colon;
    while (colon < end && isspace(*colon))
    {
        ++colon;
    }
    std::string value(colon, end);
    while(!value.empty() && isspace(value[value.size()-1]))
    {
        value.resize(value.size()-1);
    }
    _headers[key] = value;
}

