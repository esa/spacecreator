project(SpinTrailRegistrar VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES registrar.cpp)
set(includes registrar.h)

add_library(SpinTrailRegistrar STATIC)
target_sources(SpinTrailRegistrar
  PRIVATE ${SOURCES}
  ${INCLUDES})

target_include_directories(SpinTrailRegistrar
    PUBLIC .
    PRIVATE ${CMAKE_SOURCE_DIR}/src/libs)

target_link_libraries(SpinTrailRegistrar
    PRIVATE SpinTrail::Importer
    Conversion::Registry
    CommonProjectWarnings
    PUBLIC Conversion::Common)

add_format_target(SpinTrailRegistrar)
add_tidy_target(SpinTrailRegistrar)

add_library(Conversion::SpinTrailRegistrar ALIAS SpinTrailRegistrar)
