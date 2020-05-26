# Check is the qtcreator source/build exists
#
# Variables:
# QTC_FOUND (true/false)
# QTC_SOURCE_DIR (root directory of the QtCreator sourcees)
# QTC_BUILD_DIR (root directory of the QtCreator binary installation)

if($ENV{QTC_SOURCE})
    set(QTC_SOURCE_DIR $ENV{QTC_SOURCE})
else()
    # fallback
    set(QTC_SOURCE_DIR /opt/qt-creator-dev/qt-creator)
endif()
if($ENV{QTC_BUILD})
    set(QTC_BUILD_DIR $ENV{QTC_BUILD})
else()
    # fallback
    set(QTC_BUILD_DIR /opt/qt-creator-dev/build-debug)
endif()

set(QTC_FOUND TRUE)

if (NOT EXISTS "${QTC_SOURCE_DIR}/src/app/qtcreator.rc")
    message("Skipping ASN.1 plugin - could not find QtCreator sources in ${QTC_SOURCE_DIR}")
    set(QTC_FOUND FALSE)
endif()
if (NOT EXISTS "${QTC_BUILD_DIR}/lib/qtcreator/plugins/libTextEditor.so")
    message("Skipping ASN.1 plugin - could not find QtCreator build in ${QTC_BUILD_DIR}")
    set(QTC_FOUND FALSE)
endif()
