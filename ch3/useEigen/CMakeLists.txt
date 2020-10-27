cmake_minimum_required(VERSION 2.8)
project(useEigen)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "-O3")

set(THIRDPARTY_PATH "${CMAKE_CURRENT_LIST_DIR}/../../thirdparty")

# 添加Eigen头文件
include_directories("${THIRDPARTY_PATH}/eigen")
add_executable(eigenMatrix eigenMatrix.cpp)