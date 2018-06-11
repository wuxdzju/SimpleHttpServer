cmake_minimum_required(VERSION 3.5)
project(wHttpServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp epoll.h epoll.cpp http_tast.h http_tast.cpp  myerr.h myerr.cpp threadpool.h util.h util.cpp)
add_executable(wHttpServer ${SOURCE_FILES})

find_package(Threads REQUIRED)
target_link_libraries(wHttpServer  Threads::Threads)