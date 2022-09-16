#!/bin/sh
# Need to be executed before running SpaceCreator/download_qtcreator.sh

export DOWNLOAD_DIR="/home/fisker/opt/qt-creator-dev"
export ENV_QTC_VERSION_SHORT="4.8"
export ENV_QTC_VERSION="4.8.2"
export ENV_QT_BASE_DIR=${DOWNLOAD_DIR}/Qt/${ENV_QT_VERSION}/gcc_64
export ENV_QT_VERSION="5.12.12" # format: X.Y.Z

export QTC_INSTALL=${DOWNLOAD_DIR}/spacecreator.AppDir
export QTC_SOURCE=${QTC_INSTALL}
export PROJECT_ROOT="/home/fisker/projects/spacecreator"
export BUILD_DIR=${PROJECT_ROOT}/build
export CMAKE_BUILD=-DCMAKE_BUILD_TYPE=Release
export CMAKE_C_COMPILER="/usr/bin/gcc"
export PACKAGE_DIR=${PROJECT_ROOT}/package
export CMAKE_C_COMPILER=gcc # needed to ask the linker if it is the gold linker
