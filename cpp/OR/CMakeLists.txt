project(OR CXX)

find_package(Eigen3 REQUIRED)

add_library(OR INTERFACE)
target_include_directories(OR INTERFACE ${EIGEN3_INCLUDE_DIRS})