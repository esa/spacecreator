#!/bin/sh
# Build custom grantlee version with applied patch
set -e

if [ -z "${DOWNLOAD_DIR}" ]; then
    echo "DOWNLOAD_DIR not defined"
    exit 1
fi
if [ -z "${PROJECT_ROOT}" ]; then
    echo "PROJECT_ROOT not defined"
    exit 1
fi
if [ -z "${BUILD_DIR}" ]; then
    echo "BUILD_DIR not defined"
    exit 1
fi
if [ -z "${CMAKE_BUILD}" ]; then
    echo "CMAKE_BUILD not defined"
    exit 1
fi

mkdir -p ${DOWNLOAD_DIR}
cd ${DOWNLOAD_DIR}
echo "Downloading Grantlee"
curl -L https://github.com/steveire/grantlee/archive/v5.2.0.tar.gz | tar xz
cd ${BUILD_DIR}
mkdir -p ${BUILD_DIR}/grantlee
cp ${DOWNLOAD_DIR}/grantlee-*/* grantlee -r
cd ${BUILD_DIR}/grantlee

echo "Patching Grantlee"
patch -p1 < ${PROJECT_ROOT}/patches/grantlee_fix_escape.diff
mkdir -p build
cd build

echo "Building Grantlee"
cmake -DCMAKE_INSTALL_PREFIX=$(qmake -query QT_INSTALL_PREFIX) ${CMAKE_BUILD} ..
cmake --build .
cmake --build . --target install
cd ${PROJECT_ROOT}
