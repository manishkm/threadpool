cmake_minimum_required (VERSION 2.6)
project (threadpool)
add_executable(threadpool src/ThreadPool.cpp)
include_directories("./")
#SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS -std=c++11 )
target_link_libraries (threadpool pthread)
