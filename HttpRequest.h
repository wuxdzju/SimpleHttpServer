#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include "base/TimeUnit.h"

#include <string>
#include <map>
#include <assert.h>
#include <algorithm>

class Buffer;

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



    enum Version
    {
        D_UNKNOWN=0,
        D_HTTP_10,
        D_HTTP_11
    };

    HttpRequest()
            : _method(D_INVALID),
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

    const char* methodToString() const
    {
        const char* result = "UNKNOWN";
        switch (_method)
        {
            case D_GET:
                result = "GET";
                break;
            case D_POST:
                result = "POST";
                break;
            case D_HEAD:
                result = "HEAD";
                break;
            case D_PUT:
                result = "PUT";
                break;
            case D_DELETE:
                result = "DELETE";
                break;
            case D_CONNECT:
                result = "CONNECT";
                break;
            case D_OPTIONS:
                result = "OPTIONS";
                break;
            case D_PATCH:
                result = "PATCH";
                break;
            case D_TRACE:
                result = "TRACE";
                break;
            default:
                break;
        }
        return  result;
    }

    void setFilename(const char* start, const char* end)
    {
        _filename.assign(start, end);
    }

    const std::string& getFilename() const
    {
        return _filename;
    }

    void setUrl(const char* start, const char* end)
    {
        _url.assign(start, end);
    }

    const std::string& getUrl() const
    {
        return _url;
    }

    void setQuery(const char* start, const char* end)
    {
        _query.assign(start, end);
    }

    const std::string& getQuery() const
    {
        return _query;
    }

    void addHeaders(const char* start, const char* colon, const char* end);


    const std::string getHeader(const std::string& key) const
    {
        return _headers[key];
    }

    const std::map<std::string,std::string>& getHeaderMap() const
    {
        return _headers;
    };



    void setReceiveTime(TimeUnit receiveTime)
    {
        _receiveTime = receiveTime;
    }

    void swap(HttpRequest& rhs)
    {
        std::swap(_method, rhs._method);
        std::swap(_version, rhs._version);
        std::swap(_filename, rhs._filename);
        std::swap(_url, rhs._url);
        _headers.swap(rhs._headers);
        _receiveTime.swap(rhs._receiveTime);
    }


private:
    Method _method;
    Version _version;
    std::string _filename;
    std::string _url;
    std::string _query;
    std::map<std::string,std::string> _headers;
    TimeUnit _receiveTime;
};

#endif