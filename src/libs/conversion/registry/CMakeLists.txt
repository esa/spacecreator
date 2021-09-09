project(ConversionRegistry VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES registry.cpp)
set(INCLUDES registry.h)

add_library(ConversionRegistry STATIC)
target_sources(ConversionRegistry
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(ConversionRegistry
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(ConversionRegistry
    PUBLIC      Conversion::Common
    PRIVATE     CommonProjectWarnings)

add_format_target(ConversionRegistry)
add_tidy_target(ConversionRegistry)

add_library(Conversion::Registry ALIAS ConversionRegistry)
