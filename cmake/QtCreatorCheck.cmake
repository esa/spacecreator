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

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

# Verify QTC_INSTALL is available as input
if(NOT QTC_INSTALL)
    message("QTC_INSTALL was not set. Looking for environment variable")
    set(QTC_INSTALL $ENV{QTC_INSTALL})
endif()
if(QTC_INSTALL)
    message("QtCreatorCheck: Found QTC_INSTALL: " ${QTC_INSTALL})
else()
    message("QtCreatorCheck: QTC_INSTALL was NOT set.")
endif()

# Verify QTC_SOURCE is available as input
if(NOT QTC_SOURCE)
    message("QtCreatorCheck: QTC_SOURCE was not set. Looking for environment variable")
    set(QTC_SOURCE $ENV{QTC_SOURCE})
endif()
if(QTC_SOURCE)
        message("QtCreatorCheck: Found QTC_SOURCE: " ${QTC_SOURCE})
else()
    message("QtCreatorCheck: QTC_SOURCE was NOT set.")
endif()

# Find QTC_LIB_DIR from QTC_INSTALL
if(LINUX)
    find_path(QTC_LIB_DIR libExtensionSystem.so
        "${QTC_INSTALL}/lib/qtcreator/")
elseif(APPLE)
    find_path(QTC_LIB_DIR libExtensionSystem.dylib
        "${QTC_INSTALL}/Qt\ Creator.app/Contents/Frameworks")
elseif(WIN32)
    find_path(QTC_LIB_DIR ExtensionSystem.dll
        "${QTC_INSTALL}")
endif()
if (${QTC_LIB_DIR} STREQUAL "QTC_LIB_DIR-NOTFOUND")
    message(FATAL_ERROR "QtCreatorCheck: Could not find libExtensionSystem.so|dylib|dll in " ${QTC_INSTALL}/lib/qtcreator)
endif()
message("QtCreatorCheck: Looked for QTC_LIB_DIR. Found " ${QTC_LIB_DIR})

# Look for plugins dir
if(LINUX)
    find_path(QTC_PLUGINS_DIR libCore.so
        "${QTC_LIB_DIR}/plugins"
        "${QTC_INSTALL}/lib/qtcreator/plugins") # this is where libCore.so is located in QtC 4.3
elseif(APPLE)
    find_path(QTC_PLUGINS_DIR libCore.dylib
        "${QTC_INSTALL}/Qt\ Creator.app/Contents/PlugIns")
elseif(WIN32)
    find_path(QTC_PLUGINS_DIR core.dll
        "${QTC_INSTALL}/plugins")
endif()
if (${QTC_PLUGINS_DIR} STREQUAL "QTC_PLUGINS_DIR-NOTFOUND")
    message(FATAL_ERROR "QtCreatorCheck: Could not find plugins dir in ${QTC_INSTALL}/lib/qtcreator/plugins")
endif()
message("QtCreatorCheck: Looked for QTC_PLUGINS_DIR. Found " ${QTC_PLUGINS_DIR})

# Look for QTC_SOURCE_DIR
find_path(QTC_SOURCE_DIR src/libs/extensionsystem/iplugin.h
    "${QTC_INSTALL}/include/qtcreator"
    "${QTC_SOURCE}"                     # this is where iplugin.h is located in QtC 4.3
    "${QTC_SOURCE}/include/qtcreator"   # this is where iplugin.h is located in QtC 8.0.1
)
if (${QTC_SOURCE_DIR} STREQUAL "QTC_SOURCE_DIR-NOTFOUND")
    message(FATAL_ERROR "QtCreatorCheck: Could not find iplugin.h in ${QTC_SOURCE}[/include/qtcreator]/src/libs/extensionsystem/iplugin.h")
endif()
message("QtCreatorCheck: Looked for QTC_SOURCE_DIR. Found " ${QTC_SOURCE_DIR})


if (EXISTS ${QTC_SOURCE_DIR} AND EXISTS ${QTC_LIB_DIR})
    set(QTC_FOUND TRUE)
    message(STATUS "QtCreatorCheck: QtCreator sources in ${QTC_SOURCE_DIR}")
    message(STATUS "QtCreatorCheck: QtCreator libraries in ${QTC_LIB_DIR}")

    if (EXISTS "${QTC_SOURCE_DIR}/qtcreator_ide_branding.pri")
        set(priFile "${QTC_SOURCE_DIR}/qtcreator_ide_branding.pri")
    else ()
        set(priFile "${QTC_SOURCE_DIR}/qtcreator.pri")
    endif ()

    if (EXISTS ${priFile})
        file(READ ${priFile} FILE_CONTENT)
        set(_regex "QTCREATOR_COMPAT_VERSION = ([0-9.]+)")
        string(REGEX MATCH "${_regex}" _tmp "${FILE_CONTENT}")
        if (NOT _tmp)
            message(FATAL_ERROR "QtCreatorCheck: Could not detect project version number from ${priFile}")
        endif()
        set(QTC_VERSION_STR "${CMAKE_MATCH_1}")
    endif()

    set(appVersionFile "${QTC_SOURCE_DIR}/src/app/app_version.h")
    if (EXISTS ${appVersionFile})
        file(READ ${appVersionFile} FILE_CONTENT)
        set(_regex "const char IDE_VERSION_LONG\\[\\] = \"([0-9.]+)\";") # Qt > 4.10
        string(REGEX MATCH "${_regex}" _tmp "${FILE_CONTENT}")
        if (NOT _tmp)
            set(_regex "#define IDE_VERSION_DISPLAY_DEF ([0-9.]+)") # QtCreator < 4.11
            string(REGEX MATCH "${_regex}" _tmp "${FILE_CONTENT}")
            if (NOT _tmp)
                message(FATAL_ERROR "QtCreatorCheck: Could not detect project version number from ${appVersionFile}")
            endif()
        endif()
        set(QTC_VERSION_STR "${CMAKE_MATCH_1}")
    endif()

    if (NOT QTC_VERSION_STR)
        message(FATAL_ERROR "QtCreatorCheck: Unable to read QtCreator version")
    endif()
    message(STATUS "QtCreatorCheck: QtCreator (compat) version is ${QTC_VERSION_STR}")

    # Make a string that is usabe by C definitions - like 480 out of 4.8.0
    string(REGEX REPLACE "^([0-9]+)\.([0-9]+)\.([0-9]+)$" "\\1" QTC_VERSION_MAJOR ${QTC_VERSION_STR})
    MATH(EXPR QTC_VERSION_MAJOR "100 * ${QTC_VERSION_MAJOR}")
    string(REGEX REPLACE "^([0-9]+)\.([0-9]+)\.([0-9]+)$" "\\2" QTC_VERSION_MINOR ${QTC_VERSION_STR})
    MATH(EXPR QTC_VERSION "${QTC_VERSION_MAJOR} + ${QTC_VERSION_MINOR}")
endif()

if (NOT EXISTS ${QTC_SOURCE_DIR})
    message("QtCreatorCheck: No QtCreator sources found - set the environment variable QTC_SOURCE to point to the sources directory. QTC_SOURCE was " ${QTC_SOURCE})
endif()

if (NOT EXISTS ${QTC_LIB_DIR})
    message("QtCreatorCheck: No QtCreator binaries found - set the environment variable QTC_INSTALL  to point to the binary directory. QTC_INSTALL was " ${QTC_INSTALL})
endif()
