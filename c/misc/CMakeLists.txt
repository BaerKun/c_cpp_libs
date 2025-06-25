cmake_minimum_required(VERSION 3.27)

project(Algorithm)

include_directories(include)

file(GLOB SOURCES "src/*.c")

add_library(algorithm STATIC ${SOURCES})
target_include_directories(algorithm PRIVATE ../tree/include ../graph/include)
target_link_libraries(algorithm PRIVATE adjacency_list_graph bst)
