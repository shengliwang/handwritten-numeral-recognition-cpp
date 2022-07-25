cmake_minimum_required(VERSION 3.5)

project(handwritten-numeral-recognition)

set(PATH_TO_OPENMP_INSTALL "/usr")

list(APPEND CMAKE_MODULE_PATH "${PATH_TO_OPENMP_INSTALL}/lib/cmake/openmp")

message(STATUS "CMAKE_MODULE_PAHT=${CMAKE_MODULE_PATH}")
if(USINGOPENMP)
	find_package(OpenMPTarget)
	if(OPENMP_FOUND)
		message(STATUS "open mp found")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSINGOPENMP ${OpenMP_C_FLAGS}")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSINGOPENMP ${OpenMP_CXX_FLAGS}")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
	endif()
endif()


set(CMAKE_BUILD_TYPE Debug)

add_library(hnr STATIC src/matrix.cpp src/myutils.cpp src/neuralnetwork.cpp)
target_include_directories(hnr PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(test examples/test.cpp)
target_link_libraries(test PRIVATE hnr)

add_executable(mnist_train examples/mnist_train.cpp)
target_link_libraries(mnist_train PRIVATE hnr)
