file(GLOB_RECURSE SOURCES "src/graph/*.c")
add_library(graph STATIC ${SOURCES})
target_include_directories(graph PUBLIC include/public)
target_include_directories(graph PRIVATE include/private)

add_executable(test_graph _test.c)
target_include_directories(test_graph PUBLIC include/public)
target_link_libraries(test_graph PUBLIC graph)