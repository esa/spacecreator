# Check is the qtcreator source/build exists
#
# Variables:
#
# Input from the environment:
# QTC_BUILD (root directory of the QtCreator binary installation)
# QTC_SOURCE (root directory of the QtCreator sources)
#
# Output:
# QTC_FOUND (true/false)
# QTC_SOURCE_DIR (root directory of the QtCreator sources)
# QTC_LIB_DIR (Path to the directory of the qt libraries - plugins are in the "/plugins" directory)

set(QTC_FOUND FALSE)

find_path(QTC_LIB_DIR plugins/libTextEditor.so
    "$ENV{QTC_BUILD}/lib/qtcreator"
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
endif()

if (NOT EXISTS ${QTC_SOURCE_DIR})
    message("No QtCreator sources found - set the environemnt variable QTC_SOURCE to point to the sources directory")
endif()
if (NOT EXISTS ${QTC_LIB_DIR})
    message("No QtCreator binaries found - set the environemnt variable QTC_BUILD to point to the binary directory")
endif()
