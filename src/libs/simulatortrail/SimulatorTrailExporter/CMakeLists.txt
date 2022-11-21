project(SimulatorTrailExporter VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES simulatortrailexporter.cpp)
set(INCLUDES simulatortrailexporter.h)

add_library(SimulatorTrailExporter STATIC)
target_sources(SimulatorTrailExporter
  PRIVATE      "${SOURCES}"
               "${INCLUDES}")
target_include_directories(SimulatorTrailExporter
  PUBLIC                   .
  PRIVATE                  ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SimulatorTrailExporter
  PUBLIC              Conversion::Common
                      SimulatorTrail::Model
                      ${QT_CORE}
  PRIVATE             CommonProjectWarnings)

add_format_target(SimulatorTrailExporter)
add_tidy_target(SimulatorTrailExporter)

add_library(SimulatorTrail::Exporter ALIAS SimulatorTrailExporter)
