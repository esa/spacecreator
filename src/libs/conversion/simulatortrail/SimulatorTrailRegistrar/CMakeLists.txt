project(SimulatorTrailRegistrar VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES registrar.cpp)
set(includes registrar.h)

add_library(SimulatorTrailRegistrar STATIC)
target_sources(SimulatorTrailRegistrar
  PRIVATE ${SOURCES}
  ${INCLUDES})

target_include_directories(SimulatorTrailRegistrar
	PUBLIC .
	PRIVATE ${CMAKE_SOURCE_DIR}/src/libs)

target_link_libraries(SimulatorTrailRegistrar
	PRIVATE SimulatorTrail::Exporter
	Conversion::SpinTrailToSimulatorTrailTranslator
	CommonProjectWarnings
	PUBLIC Conversion::Common)

add_format_target(SimulatorTrailRegistrar)
add_tidy_target(SimulatorTrailRegistrar)

add_library(Conversion::SimulatorTrailRegistrar ALIAS SimulatorTrailRegistrar)
