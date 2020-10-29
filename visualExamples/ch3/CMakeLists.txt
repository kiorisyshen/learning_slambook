set(CH3_ROOT ${CMAKE_CURRENT_LIST_DIR})

configure_file(${CMAKE_CURRENT_LIST_DIR}/config.hpp.in ${CMAKE_CURRENT_LIST_DIR}/config.hpp)

add_executable(plotTrajectory
    plotTrajectory.cpp
)

target_link_libraries(plotTrajectory
    common
    glfw
    ${X3D_LIBRARY}
)