cmake_minimum_required(VERSION 3.6)
project(cpp_libs)

set(CMAKE_CXX_STANDARD 17)

include_directories(utils/include)
add_subdirectory(thread_pool)

add_executable(test test.cpp)