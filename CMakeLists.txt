cmake_minimum_required(VERSION 3.5)
project(sumer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")



set(SOURCE_FILES src/main.cpp
        test/echo.h
        test/echo.cpp
        src/base/noncopyable.h
        src/base/Thread.h
        src/base/Thread.cpp
        src/base/CurrentThread.h
        src/base/TimeUnit.h
        src/base/TimeUnit.cpp
        src/base/MutexLock.h
        src/EventLoop.h
        src/EventLoop.cpp
        src/Channel.h
        src/Channel.cpp
        src/Poller.cpp
        src/Poller.h
        src/Timer.h
        src/TimerManager.h
        src/TimerQueue.h
        src/TimerQueue.cpp
        src/EventLoopThread.h
        src/EventLoopThread.cpp
        src/SocketHand.h
        src/SocketHand.cpp
        src/Socket.h
        src/Socket.cpp
        src/InetAddr.h
        src/InetAddr.cpp
        src/Acceptor.h
        src/Acceptor.cpp
        src/Connection.h
        src/Connection.cpp
        src/Server.h
        src/Server.cpp
        src/Buffer.h
        src/Buffer.cpp
        src/HttpRequest.h
        src/HttpRequest.cpp
        src/HttpResponse.h
        src/HttpResponse.cpp
        src/HttpTask.h
        src/HttpTask.cpp
        src/HttpServer.h
        src/HttpServer.cpp
        src/EventLoopThreadPool.h
        src/EventLoopThreadPool.cpp
        test/test.cpp
        )

add_executable(sumer ${SOURCE_FILES})

find_package(Threads REQUIRED)
target_link_libraries(sumer  Threads::Threads)

#stess_client
project(stress_client)

add_executable(stress_client test/stress_client.cpp)

