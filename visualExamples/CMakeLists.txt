cmake_minimum_required(VERSION 3.15)
project(visualexamples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "-O3")

set(THIRDPARTY_OUT_PATH "${CMAKE_CURRENT_LIST_DIR}/../out")

# Add glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../thirdparty/glfw" "${PROJECT_BINARY_DIR}/glfw")
include_directories("${CMAKE_CURRENT_LIST_DIR}/../thirdparty/glfw/include")


# X3D (currently not public)
include_directories("${THIRDPARTY_OUT_PATH}/x3d/include")
set(X3D_LIBRARY "${THIRDPARTY_OUT_PATH}/x3d/libX3D.a")

# subdirectories
include_directories("${CMAKE_CURRENT_LIST_DIR}")
add_subdirectory(common)

add_subdirectory(ch3)