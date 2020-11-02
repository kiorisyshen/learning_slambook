/**
 * @file glfwEntry.cpp
 * @author Sijie Shen (kiorisyshen@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-10-29
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "glfwEntry.hpp"

static int winWIDTH = 800, winHEIGHT = 600;
X3D::X3DGate *g_pX3DGate;

static double cursorPosX_old;
static double cursorPosY_old;
static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
    if (ImGui::IsAnyWindowFocused()) {
        cursorPosX_old = xpos;
        cursorPosY_old = ypos;
        return;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        g_pX3DGate->RotateScreen(g_pX3DGate->GetDefaultCam(), 0.5f * float(xpos - cursorPosX_old), 0.5f * float(ypos - cursorPosY_old));
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
        g_pX3DGate->MoveScreenXY(g_pX3DGate->GetDefaultCam(), 0.03f * float(xpos - cursorPosX_old), 0.03f * float(ypos - cursorPosY_old));
    }

    cursorPosX_old = xpos;
    cursorPosY_old = ypos;
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    if (ImGui::IsAnyWindowFocused()) {
        return;
    }
    g_pX3DGate->MoveScreenZ(g_pX3DGate->GetDefaultCam(), float(yoffset));
}

GLFWwindow *glfwEntry::initialize(std::string winTittle, int winWidth, int winHeight, float bgColorR, float bgColorG, float bgColorB, X3D::X3DGate *x3dGate) {
    /* create window and GL context via GLFW */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    GLFWwindow *w = glfwCreateWindow(winWidth, winHeight, winTittle.c_str(), 0, 0);
    glfwMakeContextCurrent(w);
    glfwSwapInterval(1);

    glfwSetCursorPosCallback(w, cursor_position_callback);
    glfwSetScrollCallback(w, scroll_callback);

    X3D::X3DConfig x3dConfig;
    x3dConfig.logLvl        = X3D::LOG_LEVEL::info;
    x3dConfig.viewWidth     = winWidth;
    x3dConfig.viewHeight    = winHeight;
    x3dConfig.color_bg_r    = bgColorR;
    x3dConfig.color_bg_g    = bgColorG;
    x3dConfig.color_bg_b    = bgColorB;
    x3dConfig.loadProc_glfw = (X3D::GLFWloadproc)glfwGetProcAddress;

    g_pX3DGate = x3dGate;
    winWIDTH   = winWidth;
    winHEIGHT  = winHeight;
    g_pX3DGate->Initialize(x3dConfig);

    // imgui glfw setup
    ImGui_ImplGlfw_InitForOpenGL(w, true);

    return w;
}

void glfwEntry::beginFrame(GLFWwindow *w) {
    int cur_width, cur_height;
    glfwGetFramebufferSize(w, &cur_width, &cur_height);

    if (cur_width != winWIDTH || cur_height != winHEIGHT) {
        // Resize view
        g_pX3DGate->ResizeView(cur_width, cur_height);
        winWIDTH  = cur_width;
        winHEIGHT = cur_height;
    }

    // imgui glfw tick
    ImGui_ImplGlfw_NewFrame();
    g_pX3DGate->NewFrame_IMGUI();
}

void glfwEntry::endFrame(GLFWwindow *w) {
    g_pX3DGate->RenderOnce();
    glfwSwapBuffers(w);
    glfwPollEvents();
}

void glfwEntry::finalize() {
    ImGui_ImplGlfw_Shutdown();
    g_pX3DGate->Finalize();
    glfwTerminate();
}