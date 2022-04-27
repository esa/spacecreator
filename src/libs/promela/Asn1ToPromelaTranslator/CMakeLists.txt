project(Asn1ToPromelaTranslator VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
  translator.cpp
  promelatypesorter.cpp
  proctypemaker.cpp
  integersubset.cpp
  integergenerator.cpp
  enumeratedgenerator.cpp
  visitors/asn1nodevisitor.cpp
  visitors/asn1sequencecomponentvisitor.cpp
  visitors/integerconstraintvisitor.cpp
  visitors/asn1itemtypevisitor.cpp
  visitors/asn1nodevaluegeneratorvisitor.cpp
  visitors/asn1typevaluegeneratorvisitor.cpp
  visitors/valueassignmentvisitor.cpp
  visitors/sequencecomponentvaluevisitor.cpp
)
set(INCLUDES
  translator.h
  promelatypesorter.h
  proctypemaker.h
  integersubset.h
  integergenerator.h
  enumeratedgenerator.h
  visitors/asn1nodevisitor.h
  visitors/asn1sequencecomponentvisitor.h
  visitors/integerconstraintvisitor.h
  visitors/asn1constraintvisitor.h
  visitors/asn1itemtypevisitor.h
  visitors/asn1nodevaluegeneratorvisitor.h
  visitors/asn1typevaluegeneratorvisitor.h
  visitors/valueassignmentvisitor.h
  visitors/sequencecomponentvaluevisitor.h
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
                      Conversion::Converter
                      asn1library
                      Promela::Model
  PRIVATE             CommonProjectWarnings)

add_format_target(Asn1ToPromelaTranslator)
add_tidy_target(Asn1ToPromelaTranslator)

add_library(Conversion::Asn1ToPromelaTranslator ALIAS Asn1ToPromelaTranslator)
