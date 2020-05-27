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

# Generated files should be in the include path
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up Qt stuff:
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Concurrent Core Gui Network Svg Test WebSockets Widgets Xml REQUIRED)

# Do not use potentially conflicting Qt keywords
#add_definitions(-DQT_NO_KEYWORDS)
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
    message(STATUS "ASN.1 plugin is build")
    message(STATUS "- QTC_SOURCE_DIR ${QTC_SOURCE_DIR}")
    message(STATUS "- QTC_BUILD_DIR ${QTC_BUILD_DIR}")
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
    include(QtCreatorIDEBranding)
    include(QtCreatorAPI)
    add_subdirectory(imports/asn1scc.IDE)
endif()

# Disable the asn1 plugin for now
add_definitions(-DQT_NO_KEYWORDS)
add_subdirectory(src)
add_subdirectory(tests)
