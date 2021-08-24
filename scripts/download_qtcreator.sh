#!/bin/sh
# Build custom grantlee version with applied patch
set -e

if [ -z "${DOWNLOAD_DIR}" ]; then
    echo "DOWNLOAD_DIR not defined"
    exit 1
fi
if [ -z "${ENV_QTC_VERSION}" ]; then
    echo "ENV_QTC_VERSION not defined"
    exit 1
fi
if [ -z "${ENV_QTC_VERSION_SHORT}" ]; then
    echo "ENV_QTC_VERSION_SHORT not defined"
    exit 1
fi
if [ -z "${ENV_QT_VERSION}" ]; then
    echo "ENV_QT_VERSION not defined"
    exit 1
fi
if [ -z "${ENV_QT_BASE_DIR}" ]; then
    echo "ENV_QT_BASE_DIR not defined"
    exit 1
fi
if [ -z "${QTC_INSTALL}" ]; then
    echo "QTC_INSTALL not defined"
    exit 1
fi


mkdir -p "${DOWNLOAD_DIR}"

# Install Qt
cd "${DOWNLOAD_DIR}"
if [ ! -d "${ENV_QT_BASE_DIR}" ]; then
    echo "Installing Qt ${ENV_QT_VERSION}"
    #pip3 install -U pip
    pip3 install aqtinstall
    echo "Installing Qt ${ENV_QT_VERSION} in ${ENV_QT_BASE_DIR}"
    aqt install --outputdir "${DOWNLOAD_DIR}/Qt" --base https://download.qt.io/ ${ENV_QT_VERSION} linux desktop
fi
if [ -d "${ENV_QT_BASE_DIR}" ]; then
    echo "Qt installed in ${ENV_QT_BASE_DIR}"
else
    echo "Qt FAILED to install in ${ENV_QT_BASE_DIR}"
    exit 1
fi

LINUX_DIR="linux_64"
# if version < 4.13
if [ ${ENV_QTC_VERSION_SHORT} == "4.8" ]; then
    LINUX_DIR="linux_gcc_64_rhel72"
fi


# Grab the QtCreator binary
cd "${DOWNLOAD_DIR}"
export QTC_BINARY_FILE=qtcreator-${ENV_QTC_VERSION}.7z
if [ ! -f "${QTC_BINARY_FILE}" ]; then
    echo "Downloading ${QTC_BINARY_FILE}"
    curl -L "http://download.qt.io/official_releases/qtcreator/${ENV_QTC_VERSION_SHORT}/${ENV_QTC_VERSION}/installer_source/${LINUX_DIR}/qtcreator.7z" -o ${QTC_BINARY_FILE} -s
fi
if [ ! -d "${QTC_INSTALL}" ]; then
    echo "Extracting QtCreator binary"
    mkdir -p ${QTC_INSTALL}
    cd ${QTC_INSTALL}
    p7zip -d -k -f ../${QTC_BINARY_FILE}
fi
if [ -d "${QTC_INSTALL}" ]; then
    echo "QtCreator binary installed in ${QTC_INSTALL}"
else
    echo "QtCreator binary FAILED to install in ${QTC_INSTALL}"
    exit 1
fi

# Grab QtCreator dev
cd "${DOWNLOAD_DIR}"
export QTC_DEV_FILE=qtcreator-${ENV_QTC_VERSION}_dev.7z
if [ ! -f "${QTC_DEV_FILE}" ]; then
    echo "Downloading ${QTC_DEV_FILE}"
    curl -L "http://download.qt.io/official_releases/qtcreator/${ENV_QTC_VERSION_SHORT}/${ENV_QTC_VERSION}/installer_source/${LINUX_DIR}/qtcreator_dev.7z" -o ${QTC_DEV_FILE} -s
fi
HEADER_FILE=${QTC_INSTALL}/src/libs/extensionsystem/iplugin.h
if [ ! -f "${LIB_FILE}" ]; then
    echo "Extracting QtCreator dev"
    mkdir -p ${QTC_INSTALL}
    cd ${QTC_INSTALL}
    p7zip -d -f -k ../${QTC_DEV_FILE}
fi
if [ -f "${HEADER_FILE}" ]; then
    echo "QtCreator dev installed in ${QTC_INSTALL}"
else
    echo "QtCreator dev FAILED to install in ${QTC_INSTALL}"
    exit 1
fi
