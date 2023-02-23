project(IvToPromelaTranslator VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
  translator.cpp
  ivtopromelagenerator.cpp
  ivtopromelatranslatorcontext.cpp
  observerattachment.cpp
  systeminfo.cpp
)
set(INCLUDES
  translator.h
  ivtopromelagenerator.h
  ivtopromelatranslatorcontext.h
  observerattachment.h
  systeminfo.h
)

add_library(IvToPromelaTranslator STATIC)
target_sources(IvToPromelaTranslator
  PRIVATE      "${SOURCES}"
               "${INCLUDES}")

target_include_directories(IvToPromelaTranslator
  PUBLIC                   .
  PRIVATE                  "${CMAKE_SOURCE_DIR}/src/libs")
target_link_libraries(IvToPromelaTranslator
  PUBLIC              Conversion::Common
                      Promela::Model
                      asn1library
                      ivcore
  PRIVATE             CommonProjectWarnings)

add_format_target(IvToPromelaTranslator)
add_tidy_target(IvToPromelaTranslator)

add_library(Conversion::IvToPromelaTranslator ALIAS IvToPromelaTranslator)
