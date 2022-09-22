project(SpinTrailToSimulatorTrailTranslator VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES spintrailtosimulatortrailtranslator.cpp)
set(INCLUDES spintrailtosimulatortrailtranslator.h)

add_library(SpinTrailToSimulatorTrailTranslator STATIC)
target_sources(SpinTrailToSimulatorTrailTranslator
  PRIVATE      "${SOURCES}"
               "${INCLUDES}")

target_include_directories(SpinTrailToSimulatorTrailTranslator
  PUBLIC                   .
  PRIVATE                  "${CMAKE_SOURCE_DIR}/src/libs")
target_link_libraries(SpinTrailToSimulatorTrailTranslator
  PUBLIC              Conversion::Common
                      Conversion::Converter
                      asn1library
                      SimulatorTrail::Model
					  SpinTrail::Model
  PRIVATE             CommonProjectWarnings)

add_format_target(SpinTrailToSimulatorTrailTranslator)
add_tidy_target(SpinTrailToSimulatorTrailTranslator)

add_library(Conversion::SpinTrailToSimulatorTrailTranslator ALIAS SpinTrailToSimulatorTrailTranslator)
