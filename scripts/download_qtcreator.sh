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
if [ -z "${QTC_SOURCE}" ]; then
    echo "QTC_SOURCE not defined"
    exit 1
fi
if [ -z "${QTC_INSTALL}" ]; then
    echo "QTC_INSTALL not defined"
    exit 1
fi


mkdir -p "${DOWNLOAD_DIR}"
cd "${DOWNLOAD_DIR}"

# grab the source
export QTC_SOURCE_FILE=qt-creator-opensource-src-${ENV_QTC_VERSION}.tar.gz
if [ ! -f "${QTC_SOURCE_FILE}" ]; then
    echo "Downloading ${QTC_SOURCE_FILE}"
    curl -L "http://download.qt-project.org/official_releases/qtcreator/${ENV_QTC_VERSION_SHORT}/${ENV_QTC_VERSION}/${QTC_SOURCE_FILE}" -o ${QTC_SOURCE_FILE} -s
fi
if [ ! -d "${QTC_SOURCE}" ]; then
    echo "Extracting QtCreator source"
    tar xzf ${QTC_SOURCE_FILE}
    mv qt-creator-opensource-src-${ENV_QTC_VERSION} qt-creator
fi
if [ -d "${QTC_SOURCE}" ]; then
    echo "QtCreator source installed in ${QTC_SOURCE}"
else
    echo "QtCreator source FAILED to install in ${QTC_SOURCE}"
    exit 1
fi

# gab the binary
export QTC_BINARY_FILE=qtcreator-${ENV_QTC_VERSION}.7z
if [ ! -f "${QTC_BINARY_FILE}" ]; then
    echo "Downloading ${QTC_BINARY_FILE}"
    curl -L "http://download.qt-project.org/official_releases/qtcreator/${ENV_QTC_VERSION_SHORT}/${ENV_QTC_VERSION}/installer_source/linux_gcc_64_rhel72/qtcreator.7z" -o ${QTC_BINARY_FILE} -s
fi
if [ ! -d "${QTC_INSTALL}" ]; then
    echo "Extracting QtCreator binary"
    mkdir -p ${QTC_INSTALL}
    cd ${QTC_INSTALL}
    p7zip -d -k ../${QTC_BINARY_FILE}
fi
if [ -d "${QTC_INSTALL}" ]; then
    echo "QtCreator binary installed in ${QTC_INSTALL}"
else
    echo "QtCreator binary FAILED to install in ${QTC_INSTALL}"
    exit 1
fi

# Install Qt
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
