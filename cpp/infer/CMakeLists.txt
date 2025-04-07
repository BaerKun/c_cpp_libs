project(infer CXX)

find_package(OpenCV REQUIRED)
find_package(OpenVINO REQUIRED)

add_library(vino_infer src/vino.cpp)
target_include_directories(vino_infer PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(vino_infer PRIVATE openvino::runtime ${OpenCV_LIBS})
