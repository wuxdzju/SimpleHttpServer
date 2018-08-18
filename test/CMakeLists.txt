cmake_minimum_required(VERSION 3.5)
project(stress_client)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")




add_executable(stress_client stress_client.cpp)

find_package(Threads REQUIRED)
target_link_libraries(stress_client  Threads::Threads)