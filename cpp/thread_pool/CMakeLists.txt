cmake_minimum_required(VERSION 3.28)
project(thread_pool)

set(CMAKE_CXX_STANDARD 17)

add_library(thread_pool STATIC thread_pool.cpp)

add_executable(test test.cpp)
target_link_libraries(test PRIVATE thread_pool)
