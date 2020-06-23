# Check is the qtcreator source/build exists
#
# Variables:
#
# Input from the environment:
# QTC_INSTALL  (root directory of the QtCreator binary installation)
# QTC_SOURCE (root directory of the QtCreator sources)
#
# Output:
# QTC_FOUND (true/false)
# QTC_SOURCE_DIR (root directory of the QtCreator sources)
# QTC_LIB_DIR (Path to the directory of the qt libraries - plugins are in the "/plugins" directory)

set(QTC_FOUND FALSE)

find_path(QTC_LIB_DIR plugins/libTextEditor.so
    "$ENV{QTC_INSTALL}/lib/qtcreator"
    /opt/qt-creator-dev/build-debug/lib/qtcreator
    /usr/lib/x86_64-linux-gnu/qtcreator
    C:/Qt/qtcreator-4.9.2
)

find_path(QTC_SOURCE_DIR src/libs/extensionsystem/iplugin.h
    "$ENV{QTC_SOURCE}"
    "${QTC_SOURCE_DIR}/dev"
    /opt/qt-creator-dev/qt-creator
    "$ENV{HOME}/Qt/qt-creator"
)

if (EXISTS ${QTC_SOURCE_DIR} AND EXISTS ${QTC_LIB_DIR})
    set(QTC_FOUND TRUE)
    message(STATUS "QtCreator sources in ${QTC_SOURCE_DIR}")
    message(STATUS "QtCreator libraries in ${QTC_LIB_DIR}")

    set(priFile "${QTC_SOURCE_DIR}/qtcreator.pri")
    if (EXISTS ${priFile})
        file(READ ${priFile} FILE_CONTENT)
        set(_regex "QTCREATOR_COMPAT_VERSION = ([0-9.]+)")
        string(REGEX MATCH "${_regex}" _tmp "${FILE_CONTENT}")
        if (NOT _tmp)
            message(FATAL_ERROR "Could not detect project version number from ${priFile}")
        endif()
        set(QTC_VERSION_STR "${CMAKE_MATCH_1}")
    endif()

    set(appVersionFile "${QTC_SOURCE_DIR}/src/app/app_version.h")
    if (EXISTS ${appVersionFile})
        file(READ ${appVersionFile} FILE_CONTENT)
        set(_regex "#define IDE_VERSION_COMPAT_DEF ([0-9.]+)")
        string(REGEX MATCH "${_regex}" _tmp "${FILE_CONTENT}")
        if (NOT _tmp)
            message(FATAL_ERROR "Could not detect project version number from ${appVersionFile}")
        endif()
        set(QTC_VERSION_STR "${CMAKE_MATCH_1}")
    endif()

    if (NOT QTC_VERSION_STR)
        message(FATAL_ERROR "Unable to read QtCreator version")
    endif()
    message(STATUS "QtCreator (compat) version is ${QTC_VERSION_STR}")

    # Make a string that is usabe by C definitions - like 480 out of 4.8.0
    string(REPLACE "." "" QTC_VERSION ${QTC_VERSION_STR})
endif()

if (NOT EXISTS ${QTC_SOURCE_DIR})
    message("No QtCreator sources found - set the environemnt variable QTC_SOURCE to point to the sources directory")
endif()
if (NOT EXISTS ${QTC_LIB_DIR})
    message("No QtCreator binaries found - set the environemnt variable QTC_INSTALL  to point to the binary directory")
endif()
