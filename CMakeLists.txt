cmake_minimum_required(VERSION 3.8)

set(SC_NAME SpaceCreator)
set(SC_VERSION_MAJOR "0")
set(SC_VERSION_MINOR "1")
set(ORGANISATION ESA)
set(ORGANISATION_DOMAIN https://taste.tools)

project(${SC_NAME} VERSION "${SC_VERSION_MAJOR}.${SC_VERSION_MINOR}" LANGUAGES CXX)

add_definitions(-DSC_VERSION="${CMAKE_PROJECT_VERSION}")
add_definitions(-DSC_ORGANISATION="${ORGANISATION}")
add_definitions(-DSC_ORGANISATION_DOMAIN="${ORGANISATION_DOMAIN}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Force C++ standard, do not fall back, do not use compiler extensions
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GitHash)
include(CCache)
include(GoldLinker)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Generated files should be in the include path
set(CMAKE_INCLUDE_CURRENT_DIR ON)

option(USE_QT6 "Use Qt6 instead of Qt5 - for test purposes" OFF)

# Set up Qt stuff:
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(USE_QT6)
    find_package(Qt6 COMPONENTS Concurrent Core Gui Network Svg Test Widgets Xml REQUIRED)
    set(QT_CONCURRENT Qt6::Concurrent)
    set(QT_CORE Qt6::Core)
    set(QT_GUI Qt6::Gui)
    set(QT_NETWORK Qt6::Network)
    set(QT_SVG Qt6::Svg)
    set(QT_TEST Qt6::Test)
    set(QT_WEBSOCKETS Qt6::WebSockets)
    set(QT_WIDGETS Qt6::Widgets)
    set(QT_XML Qt6::Xml)
else()
    find_package(Qt5 COMPONENTS Concurrent Core Gui Network Svg Test WebSockets Widgets Xml REQUIRED)
    set(QT_CONCURRENT Qt5::Concurrent)
    set(QT_CORE Qt5::Core)
    set(QT_GUI Qt5::Gui)
    set(QT_NETWORK Qt5::Network)
    set(QT_SVG Qt5::Svg)
    set(QT_TEST Qt5::Test)
    set(QT_WEBSOCKETS Qt5::WebSockets)
    set(QT_WIDGETS Qt5::Widgets)
    set(QT_XML Qt5::Xml)
endif()

# Do not use outdated Qt APIs
add_definitions(-DQT_DEPRECATED_WARNINGS)

# Remove some warnings
set(CMAKE_OBJECT_PATH_MAX 500)

# Grantlee dependency
if (WIN32)
    get_target_property(QtCore_location ${QT_CORE} LOCATION)
    get_filename_component(QT_DIR ${QtCore_location} DIRECTORY)
    get_filename_component(QT_DIR ${QT_DIR} DIRECTORY)
    set(Grantlee5_DIR "${QT_DIR}/lib/cmake/Grantlee5")
    # message("Grantlee cmake dir: ${Grantlee5_DIR}")
endif()
find_package(Grantlee5 REQUIRED)

if(NOT USE_QT6)
    include(QtCreatorCheck)
    if (${QTC_FOUND})
        add_definitions(-DQTC_VERSION=${QTC_VERSION})
        if (QTC_VERSION EQUAL 48)
            message(STATUS "ASN.1 plugin is build")
        endif()
        include(QtCreatorIDEBranding)
        include(QtCreatorAPI)
    endif()
else()
    set(QTC_FOUND FALSE)
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

if (COVERAGE_ENABLED)
    include(CodeCoverage)
endif()

# The actual code
add_subdirectory(src)

# The wizards
add_subdirectory(wizards)

# Testing
option(ENABLE_TESTS "Build the tests" ON)
if(ENABLE_TESTS)
    include(TestUtils)
    enable_testing()
    add_subdirectory(tests)
endif()

# Installer data
if(UNIX)
# ASN1 compiler
add_subdirectory(asn1scc)

# install grantlee
install(FILES /usr/lib/libGrantlee_Templates.so /usr/lib/libGrantlee_Templates.so.5 /usr/lib/libGrantlee_Templates.so.5.2.0 DESTINATION /usr/lib)
install(DIRECTORY /usr/lib/grantlee DESTINATION /usr/lib)

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
# qtbase5-dev is needed to hae a proper qmake project
set (CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5gui5, libqt5svg5, libqt5widgets5, qtcreator, qtbase5-dev")
include(CPack)
endif()
