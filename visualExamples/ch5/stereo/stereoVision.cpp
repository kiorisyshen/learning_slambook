#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <Eigen/Core>
#include <unistd.h>
#include "config.hpp"

using namespace std;
using namespace Eigen;

// 文件路径
string left_file  = string(CURREXAMPLE_ROOT) + string("/left.png");
string right_file = string(CURREXAMPLE_ROOT) + string("/right.png");

void showPointCloud(
    const vector<Vector4d, Eigen::aligned_allocator<Vector4d>> &pointcloud);

int main(int argc, char **argv) {
    // 内参
    double fx = 718.856, fy = 718.856, cx = 607.1928, cy = 185.2157;
    // 基线
    double b = 0.573;

    // 读取图像
    cv::Mat left                 = cv::imread(left_file, 0);
    cv::Mat right                = cv::imread(right_file, 0);
    cv::Ptr<cv::StereoSGBM> sgbm = cv::StereoSGBM::create(
        0, 96, 9, 8 * 9 * 9, 32 * 9 * 9, 1, 63, 10, 100, 32);  // 神奇的参数
    cv::Mat disparity_sgbm, disparity;
    sgbm->compute(left, right, disparity_sgbm);
    disparity_sgbm.convertTo(disparity, CV_32F, 1.0 / 16.0f);

    // 生成点云
    vector<Vector4d, Eigen::aligned_allocator<Vector4d>> pointcloud;

    // 如果你的机器慢，请把后面的v++和u++改成v+=2, u+=2
    for (int v = 0; v < left.rows; v++)
        for (int u = 0; u < left.cols; u++) {
            if (disparity.at<float>(v, u) <= 0.0 || disparity.at<float>(v, u) >= 96.0) continue;

            Vector4d point(0, 0, 0, left.at<uchar>(v, u) / 255.0);  // 前三维为xyz,第四维为颜色

            // 根据双目模型计算 point 的位置
            double x     = (u - cx) / fx;
            double y     = (v - cy) / fy;
            double depth = fx * b / (disparity.at<float>(v, u));
            point[0]     = x * depth;
            point[1]     = y * depth;
            point[2]     = depth;

            pointcloud.push_back(point);
        }

    cv::imshow("disparity", disparity / 96.0);
    cv::waitKey(0);
    // 画出点云
    showPointCloud(pointcloud);

    return 0;
}

/*******************************************************************************************/
#include "common/glfwEntry.hpp"
X3D::X3DGate x3dGate;

void showPointCloud(const vector<Vector4d, Eigen::aligned_allocator<Vector4d>> &pointcloud) {
    if (pointcloud.empty()) {
        cerr << "Point cloud is empty!" << endl;
        return;
    }

    GLFWwindow *win = glfwEntry::initialize("stereoVision", 1024, 768, 0.2, 0.3, 0.4, &x3dGate);

    vector<float> vertPos;
    vector<float> vertColor;
    uint32_t ptsObjID = 0;

    vertPos.resize(pointcloud.size() * 3);
    vertColor.resize(vertPos.size());

    uint32_t idx = 0;
    for (auto &p : pointcloud) {
        vertPos[idx * 3]     = p[0];
        vertPos[idx * 3 + 1] = p[1];
        vertPos[idx * 3 + 2] = p[2];

        vertColor[idx * 3]     = p[3];
        vertColor[idx * 3 + 1] = p[3];
        vertColor[idx * 3 + 2] = p[3];

        ++idx;
    }

    x3dGate.AddPoints("userPoints", vertPos, vertColor, 5,
                      {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
                      [&](uint32_t objID) {
                          if (ptsObjID == 0) {
                              ptsObjID = objID;
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

    x3dGate.DeleteObject(ptsObjID, [](bool ret) {});

    glfwEntry::finalize();

    return;
}