add_executable(plotTrajectory
    plotTrajectory.cpp
)
target_link_libraries(plotTrajectory
    common
    glfw
    ${X3D_LIBRARY}
)