project(SpinTrailToSimulatorTrailTranslator VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES spintrailtosimulatortrailtranslator.cpp
	spintrailvalueparser.cpp
	octetstringprinter.cpp
	ia5stringprinter.cpp)
set(INCLUDES spintrailtosimulatortrailtranslator.h
	spintrailvalueparser.h
	octetstringprinter.h
	ia5stringprinter.h)

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
					  Conversion::Asn1ToPromelaTranslator
  PRIVATE             CommonProjectWarnings)

add_format_target(SpinTrailToSimulatorTrailTranslator)
add_tidy_target(SpinTrailToSimulatorTrailTranslator)

add_library(Conversion::SpinTrailToSimulatorTrailTranslator ALIAS SpinTrailToSimulatorTrailTranslator)
