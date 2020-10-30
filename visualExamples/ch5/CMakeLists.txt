# OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_subdirectory(imageBasics)

add_subdirectory(stereo)

add_subdirectory(rgbd)