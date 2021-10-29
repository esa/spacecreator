project(SedsSymbolDefinitionReader VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES exceptions.cpp
            symboldefinitionreader.cpp)
set(INCLUDES exceptions.h
             symboldefinitionreader.h)

add_library(SedsSymbolDefinitionReader STATIC)
target_sources(SedsSymbolDefinitionReader
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SedsSymbolDefinitionReader
    PUBLIC      .)
target_include_directories(SedsSymbolDefinitionReader SYSTEM
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SedsSymbolDefinitionReader
    PUBLIC      Conversion::Common
    PRIVATE     CommonProjectWarnings)

add_format_target(SedsSymbolDefinitionReader)
add_tidy_target(SedsSymbolDefinitionReader)

add_library(SEDS::SymbolDefinitionReader ALIAS SedsSymbolDefinitionReader)
