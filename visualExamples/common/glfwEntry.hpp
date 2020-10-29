/**
 * @file glfwEntry.hpp
 * @author Sijie Shen (kiorisyshen@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-10-29
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "glad/glad.h"
#include "x3d_linux.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"

namespace glfwEntry {
GLFWwindow *initialize(std::string winTittle, int winWidth, int winHeight, float bgColorR, float bgColorG, float bgColorB, X3D::X3DGate *x3dGate);

void beginFrame(GLFWwindow *w);

void endFrame(GLFWwindow *w);

void finalize();
}  // namespace glfwEntry