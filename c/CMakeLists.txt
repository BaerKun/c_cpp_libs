cmake_minimum_required(VERSION 3.10)

project(c_libs)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(utils ../share)
add_subdirectory(utils)
add_subdirectory(arithmetic)
add_subdirectory(misc)
add_subdirectory(geometry)

# test
#add_subdirectory(_test)

# tmp
# add_subdirectory(tmp)