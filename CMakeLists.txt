cmake_minimum_required(VERSION 3.8)

project(SpaceCreator VERSION 0.1 LANGUAGES CXX)

# Set the SC_GIT_HASH variable
find_package(Git)
if(GIT_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
    OUTPUT_VARIABLE SC_GIT_HASH
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  message(STATUS "GIT hash: ${SC_GIT_HASH}")
else()
  message(STATUS "GIT not found")
endif()
add_definitions (-DVS_BUILD_HASH="${SC_GIT_HASH}")

# Force C++ standard, do not fall back, do not use compiler extensions
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generated files should be in the include path
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Use ccache for faster builds when available
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set(CMAKE_C_COMPILER_LAUNCHER ccache)
    set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
    message("Using ccache")
endif()

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

# Disable the asn1 plugin for now
add_subdirectory(imports/asn1scc.IDE)
add_definitions(-DQT_NO_KEYWORDS)
add_subdirectory(src)
add_subdirectory(tests)
