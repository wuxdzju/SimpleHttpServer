#include "HttpResponse.h"
#include <stdio.h>
#include <string>

void HttpResponse::appendToBuffer(Buffer *outBuffer) const
{
    char buf[32];
    snprintf(buf, sizeof(buf), "HTTP/1.1 %d ",_stateCode);
    outBuffer->append(buf);
    outBuffer->append(_statusMessage);
    outBuffer->append("\r\n");

    if(_closeConnection)
    {
        outBuffer->append("Connection: close\r\n");
    }
    else
    {
        snprintf(buf, sizeof(buf), "Content-Length: %zd\r\n");
        outBuffer->append(buf);
        outBuffer->append("Connection: Keep-Alive\r\n");
    }

    for(std::map<std::string,std::string>::const_iterator it = _headers.begin();
            it != _headers.end();
            ++it)
    {
        outBuffer->append(it->first);
        outBuffer->append(": ");
        outBuffer->append(it->second);
        outBuffer->append("\r\n");
    }

    outBuffer->append("\r\n");
    outBuffer->append(_body);
}