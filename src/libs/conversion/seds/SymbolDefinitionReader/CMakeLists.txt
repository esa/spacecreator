project(SymbolDefinitionReader VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES exceptions.cpp
            symboldefinitionreader.cpp)
set(INCLUDES exceptions.h
             symboldefinitionreader.h)

add_library(SymbolDefinitionReader STATIC)
target_sources(SymbolDefinitionReader
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SymbolDefinitionReader
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs/conversion)
target_link_libraries(SymbolDefinitionReader
    PRIVATE     Conversion::Converter
    PUBLIC      Qt5::Core)

add_format_target(SymbolDefinitionReader)

add_library(SEDS::SymbolDefinitionReader ALIAS SymbolDefinitionReader)
