cmake_minimum_required(VERSION 3.18)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(SC_NAME SpaceCreator)
set(ORGANISATION ESA)
set(ORGANISATION_DOMAIN https://taste.tools)
set(SC_GIT_MAIN_BRANCH_NAME master)

include(GitHash)
include(FunctionReadVersion)
read_version("${CMAKE_CURRENT_SOURCE_DIR}/VERSION" SC_VERSION_MAJOR SC_VERSION_MINOR SC_VERSION_PATCH)
project(${SC_NAME} VERSION "${SC_VERSION_MAJOR}.${SC_VERSION_MINOR}.${SC_VERSION_PATCH}" LANGUAGES CXX)
message(STATUS "SpaceCreator Version: ${CMAKE_PROJECT_VERSION}")
include(WriteVersionFile)

add_definitions(-DSC_ORGANISATION="${ORGANISATION}")
add_definitions(-DSC_ORGANISATION_DOMAIN="${ORGANISATION_DOMAIN}")

# Force C++ standard, do not fall back, do not use compiler extensions
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CompilerWarnings)
include(CCache)
include(GoldLinker)
include(ClangFormat)
include(ClangTidy)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Generated files should be in the include path
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up Qt stuff:
find_package(Qt6 COMPONENTS Concurrent Core Core5Compat Gui Network Svg SvgWidgets Test WebSockets Widgets Xml REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_CONCURRENT Qt${QT_VERSION_MAJOR}::Concurrent)
set(QT_CORE Qt${QT_VERSION_MAJOR}::Core)
set(QT_GUI Qt${QT_VERSION_MAJOR}::Gui)
set(QT_NETWORK Qt${QT_VERSION_MAJOR}::Network)
set(QT_SVG Qt${QT_VERSION_MAJOR}::Svg)
set(QT_SVGWIDGETS Qt6::SvgWidgets)
set(QT_TEST Qt${QT_VERSION_MAJOR}::Test)
set(QT_WEBSOCKETS Qt${QT_VERSION_MAJOR}::WebSockets)
set(QT_WIDGETS Qt${QT_VERSION_MAJOR}::Widgets)
set(QT_XML Qt${QT_VERSION_MAJOR}::Xml)

# Do not use outdated Qt APIs
add_definitions(-DQT_DEPRECATED_WARNINGS)

# Remove some warnings
set(CMAKE_OBJECT_PATH_MAX 500)

# Grantlee dependency
if (WIN32)
    get_target_property(QtCore_location ${QT_CORE} LOCATION)
    get_filename_component(QT_DIR ${QtCore_location} DIRECTORY)
    get_filename_component(QT_DIR ${QT_DIR} DIRECTORY)
endif()

find_package(Grantlee5 CONFIG REQUIRED)

include(QtCreatorCheck)
if (${QTC_FOUND})
    add_definitions(-DQTC_VERSION=${QTC_VERSION})
    message(STATUS "QtCreator version is ${QTC_VERSION}")
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
    changelog.txt
    README.md
    doc/architecture.md
    doc/mainpage.md
    doc/msc_conversion_template.md
    doc/msc_remote_handler.md
    doc/quickstart.md
)

add_custom_target(BuildFiles SOURCES
    .gitattributes
    .gitignore
    .gitlab-ci.yml
    .clang-format
    .clang-tidy
    lgpl.lic.qtc
)

if (COVERAGE_ENABLED)
    include(CodeCoverage)
endif()

option(BUILD_PLUGIN_ASN1 "Build the ASN.1 Plugin" OFF)

# The actual code
add_subdirectory(src)

# The wizards
add_subdirectory(wizards)

# Testing
option(DISABLE_SLOW_TESTS "Do not include really slow tests" OFF)
option(ENABLE_TESTS "Build the tests" ON)
if(ENABLE_TESTS)
    include(TestUtils)
    enable_testing()
    add_subdirectory(tests)
endif()

# Installer data
if(UNIX)
# Packaging
set (CPACK_PACKAGE_NAME "${SC_NAME}")
set (CPACK_PACKAGE_VENDOR "European Space Agency")
set (CPACK_PACKAGE_HOMEPAGE_URL "https://taste.tools/")
set (CPACK_PACKAGE_CONTACT "Maxime Perrotin")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "QtCreator plugins for the TASTE project.")
set (CPACK_PACKAGE_VERSION_MAJOR "${SC_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${SC_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${SC_VERSION_PATCH}")
set (CPACK_PACKAGING_INSTALL_PREFIX /usr)
set (CPACK_GENERATOR "DEB")
set (CPACK_DEBIAN_PACKAGE_SECTION "Development")
# qtbase5-dev is needed to hae a proper qmake project
set (CPACK_DEBIAN_PACKAGE_DEPENDS "libqt${QT_VERSION_MAJOR}gui${QT_VERSION_MAJOR}, libqt${QT_VERSION_MAJOR}svg${QT_VERSION_MAJOR}, libqt${QT_VERSION_MAJOR}widgets${QT_VERSION_MAJOR}, qtcreator, qtbase${QT_VERSION_MAJOR}-dev")
include(CPack)
endif()
