cmake_minimum_required(VERSION 3.6)
project(cpp_libs)

set(CMAKE_CXX_STANDARD 17)

include_directories(utils/include)

add_subdirectory(socket)
add_subdirectory(thread_pool)
#add_subdirectory(infer)

add_executable(test test.cpp)
target_include_directories(test PUBLIC thread_pool infer)
target_link_libraries(test PUBLIC thread_pool my_socket)