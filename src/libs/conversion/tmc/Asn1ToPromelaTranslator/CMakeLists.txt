project(Asn1ToPromelaTranslator VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
  translator.cpp
  visitors/asn1nodevisitor.cpp
  visitors/asn1typevisitor.cpp
  visitors/asn1nodecomponentvisitor.cpp
  visitors/asn1sequencecomponentvisitor.cpp
)
set(INCLUDES
  translator.h
  visitors/asn1nodevisitor.h
  visitors/asn1typevisitor.h
  visitors/asn1nodecomponentvisitor.h
  visitors/asn1sequencecomponentvisitor.h
)

add_library(Asn1ToPromelaTranslator STATIC)
target_sources(Asn1ToPromelaTranslator
  PRIVATE      "${SOURCES}"
               "${INCLUDES}")

target_include_directories(Asn1ToPromelaTranslator
  PUBLIC                   .
  PRIVATE                  "${CMAKE_SOURCE_DIR}/src/libs")
target_link_libraries(Asn1ToPromelaTranslator
  PUBLIC              Conversion::Common
                      asn1library
                      TMC::PromelaModel
  PRIVATE             CommonProjectWarnings)

add_format_target(Asn1ToPromelaTranslator)
add_tidy_target(Asn1ToPromelaTranslator)

add_library(Conversion::Asn1ToPromelaTranslator ALIAS Asn1ToPromelaTranslator)
