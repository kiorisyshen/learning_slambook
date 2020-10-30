#!/bin/bash
set -e

PROJECT_ROOT="$(pwd)"
BUILD_TYPE=Release

mkdir -p $PROJECT_ROOT/build
rm -rf $PROJECT_ROOT/build/*

# mkdir -p $PROJECT_ROOT/out
# rm -rf $PROJECT_ROOT/out/*

# Build Eigen - (Header only though)
mkdir -p $PROJECT_ROOT/build/eigen
cd $PROJECT_ROOT/build/eigen
cmake \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DCMAKE_INSTALL_PREFIX=$PROJECT_ROOT/out \
    $PROJECT_ROOT/thirdparty/eigen

cmake --build . --config $BUILD_TYPE --target install
cd -

# Build sophus - (Header only though)
mkdir -p $PROJECT_ROOT/build/Sophus
cd $PROJECT_ROOT/build/Sophus
cmake \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DCMAKE_INSTALL_PREFIX=$PROJECT_ROOT/out \
    -DEIGEN3_INCLUDE_DIR=$PROJECT_ROOT/thirdparty/eigen \
    -DBUILD_TESTS=OFF \
    -DBUILD_EXAMPLES=OFF \
    $PROJECT_ROOT/thirdparty/Sophus

cmake --build . --config $BUILD_TYPE --target install
cd -

# Build glfw
mkdir -p $PROJECT_ROOT/build/glfw
cd $PROJECT_ROOT/build/glfw
cmake \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DCMAKE_INSTALL_PREFIX=$PROJECT_ROOT/out \
    -DGLFW_BUILD_DOCS=OFF \
    -DGLFW_BUILD_TESTS=OFF \
    -DGLFW_BUILD_EXAMPLES=OFF \
    $PROJECT_ROOT/thirdparty/glfw

cmake --build . --config $BUILD_TYPE --target install
cd -

# Build g2o
mkdir -p $PROJECT_ROOT/build/g2o
cd $PROJECT_ROOT/build/g2o
cmake \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DCMAKE_INSTALL_PREFIX=$PROJECT_ROOT/out \
    $PROJECT_ROOT/thirdparty/g2o

cmake --build . --config $BUILD_TYPE --target install -- -j8
cd -