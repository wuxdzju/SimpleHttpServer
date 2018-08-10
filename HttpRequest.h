#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include "base/TimeUnit.h"

#include <string>
#include <map>
#include <assert.h>
#include <algorithm>

class HttpRequest
{
public:
    enum Method
    {
        D_INVALID=0,
        D_GET,
        D_POST,
        D_HEAD,
        D_PUT,
        D_DELETE,
        D_TRACE,
        D_OPTIONS,
        D_CONNECT,
        D_PATCH
    };

    enum CheckState
    {
        D_CHECK_REQUESTLINE = 0,
        D_CHECK_HEADER,
        D_CHECK_CONTENT,
        D_CHECK_DONE
    };

    enum Version
    {
        D_UNKNOWN=0,
        D_HTTP_10,
        D_HTTP_11
    };

    HttpRequest()
            :_method(D_INVALID),
             _version(D_UNKNOWN)
    {

    }

    void setVersion(Version version)
    {
        _version = version;
    }

    Version getVersion() const
    {
        return _version;
    }

    bool setMethod(const char* start,const char* end);

    Method getMethod() const
    {
        return _method;
    }

    void setFilename(const char* start, const char* end)
    {
        _filename.assign(start, end);
    }

    const string& getFilename() const
    {
        return _filename;
    }

    void setUrl(const char* start, const char* end)
    {
        _url.assign(start, end);
    }

    const string& getUrl() const
    {
        return _url;
    }


private:
    Method _method;
    Version _version;
    std::string _filename;
    std::string _url;
    std::map<string,string> _headers;
    TimeUnit _receiveTime;
};

#endif