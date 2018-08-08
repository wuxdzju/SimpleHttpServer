#ifndef CALLBACK_H
#define CALLBACK_H

#include <functional>
#include <memory>

class Buffer;
class Connection;
class TimeUnit;

typedef std::function<void()> TimerCallBack;
typedef std::shared_ptr<Connection> ConnectionPtr;

typedef std::function<void( const ConnectionPtr&)> ConnectionCallBack;

//typedef std::function<void(const ConnectionPtr&,const char* data,size_t len)> MessageCallBack;
typedef std::function<void(const ConnectionPtr&,Buffer*,TimeUnit)> MessageCallBack;

typedef std::function<void(const ConnectionPtr&)> CloseCallBack;

#endif