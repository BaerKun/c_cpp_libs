add_executable(test main.c)
target_include_directories(test PRIVATE ../arithmetic/include ../geometry/include ../graph/include)
target_link_libraries(test PRIVATE arithmetic geometry_libs adjacency_list_graph)
