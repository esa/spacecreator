project(Asn1ToSedsTranslator VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES     translator.cpp
                specialized/typetranslator.cpp
                specialized/typevisitor.cpp)
set(INCLUDES    translator.h
                specialized/typetranslator.h
                specialized/typevisitor.h)

add_library(Asn1ToSedsTranslator STATIC)
target_sources(Asn1ToSedsTranslator
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(Asn1ToSedsTranslator
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(Asn1ToSedsTranslator
    PUBLIC      Conversion::Common
                asn1library
                SEDS::Model
    PRIVATE     CommonProjectWarnings)

add_format_target(Asn1ToSedsTranslator)
add_tidy_target(Asn1ToSedsTranslator)

add_library(Conversion::Asn1ToSedsTranslator ALIAS Asn1ToSedsTranslator)
