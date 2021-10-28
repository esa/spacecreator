project(Asn1ToPromelaTranslator VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
  translator.cpp
  visitors/asn1nodevisitor.cpp
  visitors/asn1sequencecomponentvisitor.cpp
  visitors/asn1sizevisitor.cpp
  visitors/asn1itemtypevisitor.cpp
)
set(INCLUDES
  translator.h
  visitors/asn1nodevisitor.h
  visitors/asn1sequencecomponentvisitor.h
  visitors/asn1sizevisitor.h
  visitors/asn1constraintvisitor.h
  visitors/asn1itemtypevisitor.h
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

add_library(TMC::Asn1ToPromelaTranslator ALIAS Asn1ToPromelaTranslator)
