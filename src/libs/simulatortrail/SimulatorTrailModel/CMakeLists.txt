project(SimulatorTrailModel VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES simulatortrailmodel.h)

set(INCLUDES simulatortrailmodel.cpp)

add_library(SimulatorTrailModel STATIC)
target_sources(SimulatorTrailModel
    PRIVATE    "${SOURCES}"
               "${INCLUDES}")
target_include_directories(SimulatorTrailModel
    PUBLIC     .
    PRIVATE    ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SimulatorTrailModel
    PUBLIC     Conversion::Common
    PRIVATE    CommonProjectWarnings)

add_format_target(SimulatorTrailModel)
add_tidy_target(SimulatorTrailModel)

add_library(SimulatorTrail::Model ALIAS SimulatorTrailModel)
