project(thread_pool)

add_library(thread_pool STATIC thread_pool.cpp)

add_executable(test test.cpp)
target_link_libraries(test PRIVATE thread_pool)
