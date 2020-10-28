#include <iostream>
#include <cmath>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <sophus/se3.hpp>

using namespace std;
using namespace Eigen;

int main() {
    // 沿Z轴转90度的旋转矩阵
    Matrix3d R = AngleAxisd(M_PI / 2, Vector3d(0, 0, 1)).toRotationMatrix();
    // 或者四元数
    Quaterniond q(R);
    Sophus::SO3d SO3_R(R);  // Sophus::SO3d可以直接从旋转矩阵构造
    Sophus::SO3d SO3_q(q);  // 也可以通过四元数构造

    // 二者是等价的
    cout << "SO(3) from matrix:\n"
         << SO3_R.matrix() << endl;
    cout << "SO(3) from quaternion:\n"
         << SO3_q.matrix() << endl;
    cout << "they are equal" << endl;

    return 0;
}