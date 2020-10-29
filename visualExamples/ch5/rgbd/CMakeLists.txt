add_executable(joinMap joinMap.cpp)

target_link_libraries(joinMap
    ${OpenCV_LIBS}
    common
    glfw
    ${X3D_LIBRARY}
)