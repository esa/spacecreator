#!/bin/sh
# Create AppImage package of SpaceCreator
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

# Grab version number(s)
source ../VERSION

# Detect system architecture to know which binaries of AppImage tools
# should be downloaded and used.
case "$(uname -i)" in
  x86_64|amd64)
#    echo "x86-64 system architecture"
    SYSTEM_ARCH="x86_64";;
  i?86)
#    echo "x86 system architecture"
    SYSTEM_ARCH="i686";;
#  arm*)
#    echo "ARM system architecture"
#    SYSTEM_ARCH="";;
  unknown|AuthenticAMD|GenuineIntel)
#         uname -i not answer on debian, then:
    case "$(uname -m)" in
      x86_64|amd64)
#        echo "x86-64 system architecture"
        SYSTEM_ARCH="x86_64";;
      i?86)
#        echo "x86 system architecture"
        SYSTEM_ARCH="i686";;
    esac ;;
  *)
    echo "Unsupported system architecture"
    exit 1;;
esac

export ARCH=${SYSTEM_ARCH}
APP_DIR=${DOWNLOAD_DIR}/spacecreator.AppDir

# Download AppImage tool
APPIMAGE_TOOL=${DOWNLOAD_DIR}/appimagetool
if [ ! -f "${APPIMAGE_TOOL}" ]; then
    URL="https://github.com/AppImage/AppImageKit/releases/download/13/appimagetool-${SYSTEM_ARCH}.AppImage"
    curl -L "${URL}" -o ${APPIMAGE_TOOL} -s
    chmod a+x ${APPIMAGE_TOOL}
fi

# Copy AppImage data
cp ${PROJECT_ROOT}/install/appimage/* ${APP_DIR}
# Copy space creator
cp ${BUILD_DIR}/bin/* ${APP_DIR}/bin
cp ${BUILD_DIR}/lib/x86_64-linux-gnu/qtcreator/plugins/* ${APP_DIR}/lib/qtcreator/plugins
cp ${BUILD_DIR}/asn1scc_bin/* ${APP_DIR} -r
cp ${PROJECT_ROOT}/wizards/files/* ${APP_DIR}/share/qtcreator/templates/wizards/files -r
cp ${PROJECT_ROOT}/wizards/projects/* ${APP_DIR}/share/qtcreator/templates/wizards/projects -r
cp ${PROJECT_ROOT}/src/qtcreator/asn1plugin/generic-highlighter/syntax/* ${APP_DIR}/share/qtcreator/generic-highlighter
cp ${PROJECT_ROOT}/src/qtcreator/asn1plugin/snippets ${APP_DIR}/share/qtcreator -r
cp ${BUILD_DIR}/grantlee/build/templates/lib/libGrantlee_Templates.* ${APP_DIR}/lib/Qt/lib
cp ${BUILD_DIR}/grantlee/build/grantlee ${APP_DIR}/lib/Qt/plugins -r
cp ${ENV_QT_BASE_DIR}/lib/libQt5WebSockets* ${APP_DIR}/lib/Qt/lib -r

# Generate AppImage
echo "Generating the app image"
${APPIMAGE_TOOL} ${APP_DIR} ${PACKAGE_DIR}/spacecreator-x86_64-${SC_VERSION}.AppImage
