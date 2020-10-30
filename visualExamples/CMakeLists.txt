cmake_minimum_required(VERSION 3.15)
project(visualexamples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "-O3")

set(THIRDPARTY_OUT_PATH "${CMAKE_CURRENT_LIST_DIR}/../out")
include_directories("${THIRDPARTY_OUT_PATH}/include/")

# Eigen
include_directories("${THIRDPARTY_OUT_PATH}/include/eigen3")

# glfw
set(glfw3_DIR "${THIRDPARTY_OUT_PATH}/lib/cmake/glfw3")
find_package(glfw3 REQUIRED)

# X3D (currently not public)
include_directories("${THIRDPARTY_OUT_PATH}/x3d/include")
set(X3D_LIBRARY "${THIRDPARTY_OUT_PATH}/x3d/libX3D.a")

# g2o
set(g2o_DIR "${THIRDPARTY_OUT_PATH}/lib/cmake/g2o")

# subdirectories
include_directories("${CMAKE_CURRENT_LIST_DIR}")
add_subdirectory(common)

add_subdirectory(ch3)

add_subdirectory(ch4)

add_subdirectory(ch5)

add_subdirectory(ch6)