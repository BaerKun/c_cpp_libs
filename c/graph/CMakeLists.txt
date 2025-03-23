file(GLOB UTILS_SOURCES "src/utils/*.c")
file(GLOB SOURCES "src/adjacency_list/*.c")
add_library(adjacency_list_graph STATIC ${UTILS_SOURCES} ${SOURCES})
target_include_directories(adjacency_list_graph PUBLIC include)
unset(UTILS_SOURCES)

file(GLOB SOURCES "src/matrix/*.c")
add_library(matrix_graph STATIC ${SOURCES})
target_include_directories(matrix_graph PUBLIC include)

file(GLOB SOURCES "src/only_edge/*.c")
add_library(only_edge_graph STATIC ${SOURCES})
target_include_directories(only_edge_graph PUBLIC include)