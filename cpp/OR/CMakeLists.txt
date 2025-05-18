project(OR CXX)

find_package(Eigen3 REQUIRED)

add_library(OR STATIC linear.cpp transport.cpp)
target_include_directories(OR PUBLIC ${EIGEN3_INCLUDE_DIRS})