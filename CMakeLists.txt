cmake_minimum_required(VERSION 3.5)
project(doker)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")



set(SOURCE_FILES main.cpp
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
        )

add_executable(doker ${SOURCE_FILES})

find_package(Threads REQUIRED)
target_link_libraries(doker  Threads::Threads)