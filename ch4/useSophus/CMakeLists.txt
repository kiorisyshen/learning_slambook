cmake_minimum_required(VERSION 2.8)
project(useSophus)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "-O3")

set(THIRDPARTY_OUT_PATH "${CMAKE_CURRENT_LIST_DIR}/../../out")

include_directories("${THIRDPARTY_OUT_PATH}/include/eigen3")
include_directories("${THIRDPARTY_OUT_PATH}/include/")

add_executable(useSophus useSophus.cpp)