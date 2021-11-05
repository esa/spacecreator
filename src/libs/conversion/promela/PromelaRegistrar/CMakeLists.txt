project(PromelaRegistrar VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES registrar.cpp)
set(INCLUDES registrar.h)

add_library(PromelaRegistrar STATIC)
target_sources(PromelaRegistrar
  PRIVATE ${SOURCES}
          ${INCLUDES})

target_include_directories(PromelaRegistrar
  PUBLIC .
  PRIVATE ${CMAKE_SOURCE_DIR}/src/libs)

target_link_libraries(PromelaRegistrar
  PRIVATE   Promela::Exporter
            Conversion::Asn1ToPromelaTranslator
  PUBLIC    Conversion::Common
            CommonProjectWarnings)

add_format_target(PromelaRegistrar)
add_tidy_target(PromelaRegistrar)

add_library(Conversion::PromelaRegistrar ALIAS PromelaRegistrar)
