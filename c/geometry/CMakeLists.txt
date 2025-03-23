cmake_minimum_required(VERSION 3.28)
project(geometry C CXX)

if (WIN32)
    if(MSVC)
        set(OpenCV_DIR "C:\\_myLibs\\opencv-4.10\\build")
    else ()
        return()
    endif ()
endif ()

find_package(OpenCV REQUIRED core imgproc highgui)

add_library(graphical_debug STATIC src/graphical.cpp)
target_link_directories(graphical_debug PUBLIC ${OpenCV_DIRS})
target_include_directories(graphical_debug PRIVATE include)
target_link_libraries(graphical_debug PRIVATE ${OpenCV_LIBS})

file(GLOB SOURCES "src/*.c")

add_library(geometry_libs STATIC ${SOURCES})
target_include_directories(geometry_libs PUBLIC include)