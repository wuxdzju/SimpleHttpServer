cmake_minimum_required(VERSION 3.5)
project(sumer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")



set(SOURCE_FILES main.cpp
        test/echo.h
        test/echo.cpp
        base/noncopyable.h
        base/Thread.h
        base/Thread.cpp
        base/CurrentThread.h
        base/TimeUnit.h
        base/TimeUnit.cpp
        base/MutexLock.h
        EventLoop.h
        EventLoop.cpp
        Channel.h
        Channel.cpp
        Poller.cpp
        Poller.h
        Timer.h
        TimerManager.h
        TimerQueue.h
        TimerQueue.cpp
        EventLoopThread.h
        EventLoopThread.cpp
        SocketHand.h
        SocketHand.cpp
        Socket.h
        Socket.cpp
        InetAddr.h
        InetAddr.cpp
        Acceptor.h
        Acceptor.cpp
        Connection.h
        Connection.cpp
        Server.h
        Server.cpp
        Buffer.h
        Buffer.cpp
        HttpRequest.h
        HttpRequest.cpp
        HttpResponse.h
        HttpResponse.cpp
        HttpTask.h
        HttpTask.cpp
        HttpServer.h
        HttpServer.cpp
        EventLoopThreadPool.h
        EventLoopThreadPool.cpp
        test.cpp
        )

add_executable(sumer ${SOURCE_FILES})

find_package(Threads REQUIRED)
target_link_libraries(sumer  Threads::Threads)