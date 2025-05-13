project(sort)

add_library(sort STATIC sort.c)

add_library(utils INTERFACE
        disjoint_set.h
        heap.h
        link_like.h
        link_node.h
        queue.h
        stack.h
        timer.h)