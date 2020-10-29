/**
 * @file plotTrajectory.cpp
 * @author Sijie Shen (kiorisyshen@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-10-29
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "common/glfwEntry.hpp"

int main() {
    X3D::X3DGate x3dGate;

    GLFWwindow *win = glfwEntry::initialize("plotTrajectory", 1024, 768, 0, 0, 0, &x3dGate);

    bool show_debug = false;
    while (!glfwWindowShouldClose(win)) {
        glfwEntry::beginFrame(win);

        {
            ImGui::Begin("Test info");

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

#ifdef DEBUG_FUNC
            if (ImGui::Button("Toggle debug")) {
                x3dGate.ToggleDebug();
                show_debug = !show_debug;
            }
#endif
            ImGui::End();
        }

        glfwEntry::endFrame(win);
    }

    glfwEntry::finalize();
}