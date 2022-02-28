###
### SET ENVIRONMENT VARS
###

export PROJECT_ROOT=$(pwd)
export ENV_QTC_VERSION_SHORT=4.8
export ENV_QTC_VERSION=4.8.2
##export ENV_QTC_VERSION_SHORT=6.0
##export ENV_QTC_VERSION=6.0.2
export ENV_QT_VERSION=5.12.1
export DOWNLOAD_DIR=/opt/qt-creator-dev
export BUILD_DIR=${PROJECT_ROOT}/build
export PACKAGE_DIR=${PROJECT_ROOT}/package
export CMAKE_BUILD=-DCMAKE_BUILD_TYPE=Release
export ENV_QT_BASE_DIR=${DOWNLOAD_DIR}/Qt/${ENV_QT_VERSION}/gcc_64
export QTC_INSTALL=${DOWNLOAD_DIR}/spacecreator.AppDir
export QTC_SOURCE=${QTC_INSTALL}

###
### CREATE FORLDERS
###

#mkdir -p ${DOWNLOAD_DIR}
#mkdir -p ${BUILD_DIR}
#mkdir -p ${PACKAGE_DIR}

###
### INSTALL REQUIRED SYSTEM PACKAGES
###

#apt install -y  build-essential git g++ cmake cmake-data ninja-build make gcovr lcov ccache cccc doxygen graphviz zip p7zip curl wget default-jre uuid-dev pkg-config fuse python3-pip

###
### DOWNLOAD QTCREATOR AND GRANTLEE
###

#${PROJECT_ROOT}/scripts/download_qtcreator.sh
#${PROJECT_ROOT}/scripts/build_grantlee.sh

###
### BUILD SPACE CREATOR
###

cd ${BUILD_DIR}
cmake -GNinja -S ${PROJECT_ROOT} -B ${BUILD_DIR} ${CMAKE_BUILD} -DBUILD_PATCH_NUMBER=$CI_BUILD_ID -DCMAKE_PREFIX_PATH:STRING=${ENV_QT_BASE_DIR} -DQT_QMAKE_EXECUTABLE:STRING=${ENV_QT_BASE_DIR}/bin/qmake
cmake --build . --target all

###
### CREATE APP IMAGE
###
${PROJECT_ROOT}/scripts/create_appimage.sh

###
### RUN TESTS
###

#cd ${DOWNLOAD_DIR}
#wget -q -O - https://github.com/ttsiodras/asn1scc/releases/download/4.2.4.7f/asn1scc-bin-4.2.4.7f.tar.bz2 | tar jxvf -
#export PATH=$PATH:${DOWNLOAD_DIR}/asn1scc
#cd ${PROJECT_ROOT}
#./scripts/run_tests.sh ${BUILD_DIR}
