project(Asn1ToPromelaTranslator VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
  enumeratedgenerator.cpp
  integergenerator.cpp
  integersubset.cpp
  realgenerator.cpp
  realsubset.cpp
  proctypemaker.cpp
  promelatypesorter.cpp
  translator.cpp
  visitors/asn1itemtypevisitor.cpp
  visitors/asn1nodevaluegeneratorvisitor.cpp
  visitors/asn1nodevisitor.cpp
  visitors/asn1sequencecomponentvisitor.cpp
  visitors/asn1typevaluegeneratorvisitor.cpp
  visitors/enumvalueconstraintvisitor.cpp
  visitors/integerrangeconstraintvisitor.cpp
  visitors/realrangeconstraintvisitor.cpp
  visitors/sequencecomponentvaluevisitor.cpp
  visitors/valueassignmentvisitor.cpp
)
set(INCLUDES
  enumeratedgenerator.h
  integergenerator.h
  integersubset.h
  proctypemaker.h
  promelatypesorter.h
  realgenerator.h
  realsubset.h
  translator.h
  visitors/asn1itemtypevisitor.h
  visitors/asn1nodevaluegeneratorvisitor.h
  visitors/asn1nodevisitor.h
  visitors/asn1sequencecomponentvisitor.h
  visitors/asn1typevaluegeneratorvisitor.h
  visitors/enumvalueconstraintvisitor.h
  visitors/integerrangeconstraintvisitor.h
  visitors/realrangeconstraintvisitor.h
  visitors/sequencecomponentvaluevisitor.h
  visitors/sizeconstraintvisitor.h
  visitors/valueassignmentvisitor.h
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
                      ivcore
                      Promela::Model
                      Promela::Common
  PRIVATE             CommonProjectWarnings)

add_format_target(Asn1ToPromelaTranslator)
add_tidy_target(Asn1ToPromelaTranslator)

add_library(Conversion::Asn1ToPromelaTranslator ALIAS Asn1ToPromelaTranslator)
