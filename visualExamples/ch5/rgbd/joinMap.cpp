#include <iostream>
#include <fstream>

#include <opencv2/opencv.hpp>
#include <boost/format.hpp>  // for formating strings
#include <sophus/se3.hpp>

#include "config.hpp"

using namespace std;

typedef vector<Sophus::SE3d, Eigen::aligned_allocator<Sophus::SE3d>> TrajectoryType;
typedef Eigen::Matrix<double, 6, 1> Vector6d;

void showPointCloud(const vector<Vector6d, Eigen::aligned_allocator<Vector6d>> &pointcloud);

int main(int argc, char **argv) {
    vector<cv::Mat> colorImgs, depthImgs;  // 彩色图和深度图
    TrajectoryType poses;                  // 相机位姿

    ifstream fin((string(CURREXAMPLE_ROOT) + string("/pose.txt")).c_str());
    if (!fin) {
        cerr << "请在有pose.txt的目录下运行此程序" << endl;
        return 1;
    }

    for (int i = 0; i < 5; i++) {
        boost::format fmt((string(CURREXAMPLE_ROOT) + string("/%s/%d.%s")).c_str());  //图像文件格式
        colorImgs.push_back(cv::imread((fmt % "color" % (i + 1) % "png").str()));
        depthImgs.push_back(cv::imread((fmt % "depth" % (i + 1) % "pgm").str(), -1));  // 使用-1读取原始图像

        double data[7] = {0};
        for (auto &d : data)
            fin >> d;
        Sophus::SE3d pose(Eigen::Quaterniond(data[6], data[3], data[4], data[5]),
                          Eigen::Vector3d(data[0], data[1], data[2]));
        poses.push_back(pose);
    }

    // 计算点云并拼接
    // 相机内参
    double cx         = 325.5;
    double cy         = 253.5;
    double fx         = 518.0;
    double fy         = 519.0;
    double depthScale = 1000.0;
    vector<Vector6d, Eigen::aligned_allocator<Vector6d>> pointcloud;
    pointcloud.reserve(1000000);

    for (int i = 0; i < 5; i++) {
        cout << "转换图像中: " << i + 1 << endl;
        cv::Mat color  = colorImgs[i];
        cv::Mat depth  = depthImgs[i];
        Sophus::SE3d T = poses[i];
        for (int v = 0; v < color.rows; v++)
            for (int u = 0; u < color.cols; u++) {
                unsigned int d = depth.ptr<unsigned short>(v)[u];  // 深度值
                if (d == 0) continue;                              // 为0表示没有测量到
                Eigen::Vector3d point;
                point[2]                   = double(d) / depthScale;
                point[0]                   = (u - cx) * point[2] / fx;
                point[1]                   = (v - cy) * point[2] / fy;
                Eigen::Vector3d pointWorld = T * point;

                Vector6d p;
                p.head<3>() = pointWorld;
                p[5]        = color.data[v * color.step + u * color.channels()];      // blue
                p[4]        = color.data[v * color.step + u * color.channels() + 1];  // green
                p[3]        = color.data[v * color.step + u * color.channels() + 2];  // red
                pointcloud.push_back(p);
            }
    }

    cout << "点云共有" << pointcloud.size() << "个点." << endl;
    showPointCloud(pointcloud);
    return 0;
}

/*******************************************************************************************/
#include "common/glfwEntry.hpp"
X3D::X3DGate x3dGate;

void showPointCloud(const vector<Vector6d, Eigen::aligned_allocator<Vector6d>> &pointcloud) {
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

        vertColor[idx * 3]     = p[3] / 255.0;
        vertColor[idx * 3 + 1] = p[4] / 255.0;
        vertColor[idx * 3 + 2] = p[5] / 255.0;

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