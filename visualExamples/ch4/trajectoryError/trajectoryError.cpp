/**
 * @file trajectoryError.cpp
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
#include <sophus/se3.hpp>

#include <iostream>
#include <fstream>

using namespace Sophus;
using namespace std;

string groundtruth_file = string(CURREXAMPLE_ROOT) + string("/groundtruth.txt");
string estimated_file   = string(CURREXAMPLE_ROOT) + string("/estimated.txt");

typedef vector<Sophus::SE3d, Eigen::aligned_allocator<Sophus::SE3d>> TrajectoryType;

TrajectoryType ReadTrajectory(const string &path);

// drawing with x3d
X3D::X3DGate x3dGate;

void DrawTrajectory(const TrajectoryType &gt, const TrajectoryType &esti);

int main() {
    TrajectoryType groundtruth = ReadTrajectory(groundtruth_file);
    TrajectoryType estimated   = ReadTrajectory(estimated_file);
    assert(!groundtruth.empty() && !estimated.empty());
    assert(groundtruth.size() == estimated.size());

    // compute rmse
    double rmse = 0;
    for (size_t i = 0; i < estimated.size(); i++) {
        Sophus::SE3d p1 = estimated[i], p2 = groundtruth[i];
        double error = (p2.inverse() * p1).log().norm();
        rmse += error * error;
    }
    rmse = rmse / double(estimated.size());
    rmse = sqrt(rmse);
    cout << "RMSE = " << rmse << endl;

    DrawTrajectory(groundtruth, estimated);
}

/*******************************************************************************************/
TrajectoryType ReadTrajectory(const string &path) {
    ifstream fin(path);
    TrajectoryType trajectory;
    if (!fin) {
        cerr << "trajectory " << path << " not found." << endl;
        return trajectory;
    }

    while (!fin.eof()) {
        double time, tx, ty, tz, qx, qy, qz, qw;
        fin >> time >> tx >> ty >> tz >> qx >> qy >> qz >> qw;
        Sophus::SE3d p1(Eigen::Quaterniond(qw, qx, qy, qz), Eigen::Vector3d(tx, ty, tz));
        trajectory.push_back(p1);
    }
    return trajectory;
}

void DrawTrajectory(const TrajectoryType &gt, const TrajectoryType &esti) {
    GLFWwindow *win = glfwEntry::initialize("trajectoryError", 1024, 768, 0, 0, 0, &x3dGate);

    vector<float> vertPos;
    vector<float> vertColor;
    uint32_t trajObjID = 0;

    uint32_t esti_offset = (gt.size() - 1) * 6;

    vertPos.resize(esti_offset + (esti.size() - 1) * 6);
    vertColor.resize(vertPos.size());
    fill(vertColor.begin(), vertColor.end(), 0.0);

    for (size_t i = 0; i < gt.size() - 1; i++) {
        auto p1 = gt[i], p2 = gt[i + 1];
        vertPos[i * 6]       = p1.translation()[0];
        vertPos[i * 6 + 1]   = p1.translation()[1];
        vertPos[i * 6 + 2]   = p1.translation()[2];
        vertPos[i * 6 + 3]   = p2.translation()[0];
        vertPos[i * 6 + 4]   = p2.translation()[1];
        vertPos[i * 6 + 5]   = p2.translation()[2];
        vertColor[i * 6 + 2] = 1.0;  // blue for ground truth
        vertColor[i * 6 + 5] = 1.0;
    }

    for (size_t i = 0; i < esti.size() - 1; i++) {
        auto p1 = esti[i], p2 = esti[i + 1];

        vertPos[esti_offset + i * 6]       = p1.translation()[0];
        vertPos[esti_offset + i * 6 + 1]   = p1.translation()[1];
        vertPos[esti_offset + i * 6 + 2]   = p1.translation()[2];
        vertPos[esti_offset + i * 6 + 3]   = p2.translation()[0];
        vertPos[esti_offset + i * 6 + 4]   = p2.translation()[1];
        vertPos[esti_offset + i * 6 + 5]   = p2.translation()[2];
        vertColor[esti_offset + i * 6]     = 1.0;  // red for ground truth
        vertColor[esti_offset + i * 6 + 3] = 1.0;
    }

    x3dGate.AddLines("gt_and_esti", vertPos, vertColor,
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