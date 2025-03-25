add_executable(test main.c)
target_include_directories(test PRIVATE ../arithmetic/include ../geometry/include ../graph/include ../tree/include)
target_link_libraries(test PRIVATE bst)
