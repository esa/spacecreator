project(TmcRegistrar VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES registrar.cpp)
set(INCLUDES registrar.h)

add_library(TmcRegistrar STATIC)
target_sources(TmcRegistrar
  PRIVATE ${SOURCES}
          ${INCLUDES})

target_include_directories(TmcRegistrar
  PUBLIC .
  PRIVATE ${CMAKE_SOURCE_DIR}/src/libs)

target_link_libraries(TmcRegistrar
  PRIVATE   Conversion::PromelaExporter
            TMC::Asn1ToPromelaTranslator
  PUBLIC    Conversion::Common
            CommonProjectWarnings)

add_format_target(TmcRegistrar)
add_tidy_target(TmcRegistrar)

add_library(Conversion::TmcRegistrar ALIAS TmcRegistrar)
