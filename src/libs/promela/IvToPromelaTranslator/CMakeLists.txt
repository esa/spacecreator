project(IvToPromelaTranslator VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
  ccodegenerator.cpp
  helper.cpp
  ivtopromelagenerator.cpp
  ivtopromelatranslatorcontext.cpp
  observerattachment.cpp
  sequencecomponentvisitor.cpp
  systeminfo.cpp
  translator.cpp
)
set(INCLUDES
  ccodegenerator.h
  helper.h
  ivtopromelagenerator.h
  ivtopromelatranslatorcontext.h
  observerattachment.h
  sequencecomponentvisitor.h
  systeminfo.h
  translator.h
)

add_library(IvToPromelaTranslator STATIC)
target_sources(IvToPromelaTranslator
  PRIVATE      "${SOURCES}"
               "${INCLUDES}")

target_include_directories(IvToPromelaTranslator
  PUBLIC                   .
  PRIVATE                  "${CMAKE_SOURCE_DIR}/src/libs")
target_link_libraries(IvToPromelaTranslator
    PUBLIC            Conversion::Common
                      Conversion::Asn1ToPromelaTranslator
                      Promela::Model
                      Promela::Common
                      asn1library
                      ivcore
  PRIVATE             CommonProjectWarnings)

add_format_target(IvToPromelaTranslator)
add_tidy_target(IvToPromelaTranslator)

add_library(Conversion::IvToPromelaTranslator ALIAS IvToPromelaTranslator)
