cmake_minimum_required(VERSION 3.8)

set(SC_NAME SpaceCreator)
set(SC_VERSION_MAJOR "0")
set(SC_VERSION_MINOR "1")

project(${SC_NAME} VERSION "${SC_VERSION_MAJOR}.${SC_VERSION_MINOR}" LANGUAGES CXX)

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

include(QtCreatorCheck)
if (${QTC_FOUND})
    add_definitions(-DQTC_VERSION=${QTC_VERSION})
    message(STATUS "ASN.1 plugin is build")
    include(QtCreatorIDEBranding)
    include(QtCreatorAPI)
endif()

find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(documentation ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/doc/Doxygen
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/doc
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif()
add_custom_target(DocuFiles SOURCES
    README.md
    doc/architecture.md
    doc/mainpage.md
    doc/msc_conversion_template.md
    doc/msc_remote_handler.md
)

add_custom_target(BuildFiles SOURCES
    .gitattributes
    .gitignore
    .gitlab-ci.yml
    _clang-format
    _clang-tidy
    lgpl.lic.qtc
)

# The actual code
add_subdirectory(src)

# Testing
option(ENABLE_TESTS "Build the tests" ON)
if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Installer data
if(UNIX)
set (CPACK_PACKAGE_NAME "${SC_NAME}")
set (CPACK_PACKAGE_VENDOR "European Space Agency")
set (CPACK_PACKAGE_HOMEPAGE_URL "https://taste.tools/")
set(CPACK_PACKAGE_CONTACT "Maxime Perrotin")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "QtCrator plugins for the TASTE project.")
set (CPACK_PACKAGE_VERSION_MAJOR "${SC_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${SC_VERSION_MINOR}")
if(DEFINED BUILD_PATCH_NUMBER)
    set (CPACK_PACKAGE_VERSION_PATCH "${BUILD_PATCH_NUMBER}")
else()
    set (CPACK_PACKAGE_VERSION_PATCH "${SC_GIT_HASH}")
endif()
set (CPACK_PACKAGING_INSTALL_PREFIX /usr)
set (CPACK_GENERATOR "DEB")
set (CPACK_DEBIAN_PACKAGE_SECTION "Development")
set (CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5gui5, libqt5svg5, libqt5widgets5, qtcreator")
include(CPack)
endif()
