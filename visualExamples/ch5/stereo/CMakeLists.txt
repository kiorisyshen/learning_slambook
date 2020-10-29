add_executable(stereoVision stereoVision.cpp)

target_link_libraries(stereoVision
    ${OpenCV_LIBS}
    common
    glfw
    ${X3D_LIBRARY}
)