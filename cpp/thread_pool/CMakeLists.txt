project(thread_pool)

add_library(thread_pool STATIC thread_pool.cpp)
add_executable(tt _test.cpp)
target_link_libraries(tt PRIVATE thread_pool)