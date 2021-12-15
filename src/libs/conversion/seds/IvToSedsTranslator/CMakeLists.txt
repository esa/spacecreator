project(IvToSedsTranslator VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES     translator.cpp
                specialized/functionstranslator.cpp)
set(INCLUDES    translator.h
                specialized/functionstranslator.h)

add_library(IvToSedsTranslator STATIC)
target_sources(IvToSedsTranslator
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(IvToSedsTranslator
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(IvToSedsTranslator
    PUBLIC      Conversion::Common
                ivcore
                libiveditor
                SEDS::Model
    PRIVATE     CommonProjectWarnings)

add_format_target(IvToSedsTranslator)
add_tidy_target(IvToSedsTranslator)

add_library(Conversion::IvToSedsTranslator ALIAS IvToSedsTranslator)
