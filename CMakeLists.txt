cmake_minimum_required(VERSION 2.9)

project(handwritten-numeral-recognition)


set(CMAKE_BUILD_TYPE Debug)

add_library(hnr STATIC src/matrix.cpp src/myutils.cpp src/neuralnetwork.cpp)
target_include_directories(hnr PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(test src/test/test.cpp)
target_link_libraries(test PRIVATE hnr)

