cmake_minimum_required(VERSION 3.8)

project(SpaceCreator VERSION 0.1 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Force C++ standard, do not fall back, do not use compiler extensions
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GitHash)
include(CCache)
include(GoldLinker)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Generated files should be in the include path
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up Qt stuff:
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Concurrent Core Gui Network Svg Test WebSockets Widgets Xml REQUIRED)

# Do not use outdated Qt APIs
add_definitions(-DQT_DEPRECATED_WARNINGS)

# Remove some warnings
set(CMAKE_OBJECT_PATH_MAX 500)

# Grantlee dependency
if (WIN32)
    get_target_property(QtCore_location Qt5::Core LOCATION)
    get_filename_component(QT_DIR ${QtCore_location} DIRECTORY)
    get_filename_component(QT_DIR ${QT_DIR} DIRECTORY)
    set(Grantlee5_DIR "${QT_DIR}/lib/cmake/Grantlee5")
    # message("Grantlee cmake dir: ${Grantlee5_DIR}")
endif()
find_package(Grantlee5 REQUIRED)

enable_testing()

include(QtCreatorCheck)
if (${QTC_FOUND})
    add_definitions(-DQTC_VERSION=${QTC_VERSION})
    message(STATUS "ASN.1 plugin is build")
    include(QtCreatorIDEBranding)
    include(QtCreatorAPI)
    add_subdirectory(imports/asn1scc.IDE)
endif()

find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(documentation ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/doc/Doxygen
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/doc
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif()
add_custom_target(DocuFiles SOURCES README.md doc/mainpage.md doc/architecture.md doc/msc_remote_handler.md)

add_custom_target(BuildFiles SOURCES .gitlab-ci.yml _clang-format _clang-tidy lgpl.lic.qtc .gitattributes .gitignore)

# Disable the asn1 plugin for now
add_subdirectory(src)
add_subdirectory(tests)
