file(GLOB SOURCES "src/adjacency_list/*.c" "src/matrix/*.c" "src/only_edge/*.c" "src/flow_network/*.c")
add_library(graph STATIC ${SOURCES})
target_include_directories(graph PUBLIC include)

add_executable(test_graph _test.c)
target_include_directories(test_graph PUBLIC include)
target_link_libraries(test_graph PUBLIC graph)