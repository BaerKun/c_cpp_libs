file(GLOB SOURCES  "src/matrix/*.c" "src/edge_set/*.c" "src/share/*.c")
add_library(graph STATIC ${SOURCES})
target_include_directories(graph PUBLIC include)

add_executable(test_graph _test.c)
target_include_directories(test_graph PUBLIC include)
target_link_libraries(test_graph PUBLIC graph)