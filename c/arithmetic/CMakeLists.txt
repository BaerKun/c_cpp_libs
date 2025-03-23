project(Arithmetic)

file(GLOB SOURCES "src/*.c" )

add_library(arithmetic ${SOURCES})

target_include_directories(arithmetic PRIVATE include)

if (UNIX)
    target_link_libraries(arithmetic PUBLIC m)
endif ()