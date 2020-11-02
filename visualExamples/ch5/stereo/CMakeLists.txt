add_executable(stereoVision stereoVision.cpp)

set(CURREXAMPLE_ROOT ${CMAKE_CURRENT_LIST_DIR})
configure_file(${CMAKE_CURRENT_LIST_DIR}/config.hpp.in ${CMAKE_CURRENT_LIST_DIR}/config.hpp)

target_link_libraries(stereoVision
    ${OpenCV_LIBS}
    common
    glfw
    ${X3D_LIBRARY}
)