project(Asn1ToPromelaTranslator VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
  translator.cpp
  promelatypesorter.cpp
  integersubset.cpp
  integergenerator.cpp
  visitors/asn1nodevisitor.cpp
  visitors/asn1sequencecomponentvisitor.cpp
  visitors/integerconstraintvisitor.cpp
  visitors/asn1itemtypevisitor.cpp
  visitors/asn1nodevaluegeneratorvisitor.cpp
  visitors/asn1typevaluegeneratorvisitor.cpp
)
set(INCLUDES
  translator.h
  promelatypesorter.h
  integersubset.h
  integergenerator.h
  visitors/asn1nodevisitor.h
  visitors/asn1sequencecomponentvisitor.h
  visitors/integerconstraintvisitor.h
  visitors/asn1constraintvisitor.h
  visitors/asn1itemtypevisitor.h
  visitors/asn1nodevaluegeneratorvisitor.h
  visitors/asn1typevaluegeneratorvisitor.h
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
                      Promela::Model
  PRIVATE             CommonProjectWarnings)

add_format_target(Asn1ToPromelaTranslator)
add_tidy_target(Asn1ToPromelaTranslator)

add_library(Conversion::Asn1ToPromelaTranslator ALIAS Asn1ToPromelaTranslator)
