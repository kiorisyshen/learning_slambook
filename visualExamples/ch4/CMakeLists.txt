set(CURREXAMPLE_ROOT ${CMAKE_CURRENT_LIST_DIR})

configure_file(${CMAKE_CURRENT_LIST_DIR}/config.hpp.in ${CMAKE_CURRENT_LIST_DIR}/config.hpp)

add_executable(trajectoryError
    trajectoryError.cpp
)

target_link_libraries(trajectoryError
    common
    glfw
    ${X3D_LIBRARY}
)