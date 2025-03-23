project(Tree)

set(CMAKE_C_STANDARD 11)

include_directories(include)

file(GLOB SOURCES "src/*.c")

add_library(bst STATIC ${SOURCES})
target_compile_definitions(bst PUBLIC TREE_BINARY)