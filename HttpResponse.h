#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <string>
#include <map>

#include "Buffer.h"

class HttpResponse
{
public:
    enum HttpStatusCode
    {
        D_UNKNOWN,
        D_200_OK = 200,
        D_301_MOVE_PERMANENTLY = 301,
        D_400_BAD_REQUEST = 400,
        D_403_FORBID = 403,
        D_404_NOT_FOUND = 404,
        D_500_INTERNAL_ERROR = 500
    };



    explicit HttpResponse(bool closeConnection)
            :_closeConnection(closeConnection)
    {

    }

    void setStatusCode(HttpStatusCode statusCode)
    {
        _stateCode = statusCode;
    }

    void setStatusMessage(const std::string& message)
    {
        _statusMessage = message;
    }

    void setCloseConnection(bool close)
    {
        _closeConnection = close;
    }

    bool closeConnection() const
    {
        return _closeConnection;
    }

    void addHeader(const std::string& key, const std::string& value)
    {
        _headers[key] = value;
    }

    void setContentType(const std::string& contentType)
    {
        addHeader("Content-Type",contentType);
    }

    void setBody(std::string body)
    {
        _body = body;
    }

    void appendToBuffer(Buffer* outBuffer) const;

private:
    HttpStatusCode _stateCode;
    std::string _statusMessage;
    std::map<std::string, std::string> _headers;
    std::string _body;
    bool _closeConnection;
};

#endif