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
#include "config.hpp"

#include "common/glfwEntry.hpp"
#include <Eigen/Dense>

#include <iostream>
#include <fstream>

using namespace std;
using namespace Eigen;

// path to trajectory file
string trajectory_file = string(CH3_ROOT) + string("/trajectory.txt");
// drawing with x3d
X3D::X3DGate x3dGate;

void DrawTrajectory(vector<Isometry3d, Eigen::aligned_allocator<Isometry3d>>);

int main() {
    vector<Isometry3d, Eigen::aligned_allocator<Isometry3d>> poses;
    cout << "trajectory_file: " << endl;
    ifstream fin(trajectory_file);
    if (!fin) {
        cout << "cannot find trajectory file at " << trajectory_file << endl;
        return 1;
    }

    while (!fin.eof()) {
        double time, tx, ty, tz, qx, qy, qz, qw;
        fin >> time >> tx >> ty >> tz >> qx >> qy >> qz >> qw;
        Isometry3d Twr(Quaterniond(qw, qx, qy, qz));
        Twr.pretranslate(Vector3d(tx, ty, tz));
        poses.push_back(Twr);
    }
    cout << "read total " << poses.size() << " pose entries" << endl;

    DrawTrajectory(poses);
}

/*******************************************************************************************/
void DrawTrajectory(vector<Isometry3d, Eigen::aligned_allocator<Isometry3d>> poses) {
    GLFWwindow *win = glfwEntry::initialize("plotTrajectory", 1024, 768, 0.2, 0.3, 0.4, &x3dGate);

    vector<float> vertPos;
    vector<float> vertColor;
    uint32_t trajObjID = 0;

    vertPos.resize(poses.size() * 18 + poses.size() * 6);
    vertColor.resize(vertPos.size());
    fill(vertColor.begin(), vertColor.end(), 0.0);

    for (size_t i = 0; i < poses.size(); i++) {
        // 画每个位姿的三个坐标轴
        Vector3d Ow = poses[i].translation();
        Vector3d Xw = poses[i] * (0.1 * Vector3d(1, 0, 0));
        Vector3d Yw = poses[i] * (0.1 * Vector3d(0, 1, 0));
        Vector3d Zw = poses[i] * (0.1 * Vector3d(0, 0, 1));

        vertPos[i * 18]       = Ow[0];
        vertPos[i * 18 + 1]   = Ow[1];
        vertPos[i * 18 + 2]   = Ow[2];
        vertPos[i * 18 + 3]   = Xw[0];
        vertPos[i * 18 + 4]   = Xw[1];
        vertPos[i * 18 + 5]   = Xw[2];
        vertColor[i * 18]     = 1.0;
        vertColor[i * 18 + 3] = 1.0;

        vertPos[i * 18 + 6]    = Ow[0];
        vertPos[i * 18 + 7]    = Ow[1];
        vertPos[i * 18 + 8]    = Ow[2];
        vertPos[i * 18 + 9]    = Yw[0];
        vertPos[i * 18 + 10]   = Yw[1];
        vertPos[i * 18 + 11]   = Yw[2];
        vertColor[i * 18 + 7]  = 1.0;
        vertColor[i * 18 + 10] = 1.0;

        vertPos[i * 18 + 12]   = Ow[0];
        vertPos[i * 18 + 13]   = Ow[1];
        vertPos[i * 18 + 14]   = Ow[2];
        vertPos[i * 18 + 15]   = Zw[0];
        vertPos[i * 18 + 16]   = Zw[1];
        vertPos[i * 18 + 17]   = Zw[2];
        vertColor[i * 18 + 14] = 1.0;
        vertColor[i * 18 + 17] = 1.0;
    }
    uint32_t posLineOffset = poses.size() * 18;
    // 画出连线
    for (size_t i = 0; i < poses.size(); i++) {
        auto p1 = poses[i], p2 = poses[i + 1];

        vertPos[posLineOffset + i * 6]     = p1.translation()[0];
        vertPos[posLineOffset + i * 6 + 1] = p1.translation()[1];
        vertPos[posLineOffset + i * 6 + 2] = p1.translation()[2];
        vertPos[posLineOffset + i * 6 + 3] = p2.translation()[0];
        vertPos[posLineOffset + i * 6 + 4] = p2.translation()[1];
        vertPos[posLineOffset + i * 6 + 5] = p2.translation()[2];
    }

    x3dGate.AddLines("trajectory_file", vertPos, vertColor,
                     {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
                     [&](uint32_t objID) {
                         if (trajObjID == 0) {
                             trajObjID = objID;
                         }
                     });

    bool show_debug = false;
    while (!glfwWindowShouldClose(win)) {
        glfwEntry::beginFrame(win);

        {
            ImGui::Begin("Test info");

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            if (ImGui::Button("Toggle debug")) {
                x3dGate.ToggleDebug();
                show_debug = !show_debug;
            }
            ImGui::End();
        }

        glfwEntry::endFrame(win);
    }

    x3dGate.DeleteObject(trajObjID, [](bool ret) {});

    glfwEntry::finalize();
}