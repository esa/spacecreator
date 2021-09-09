project(ConversionConverter VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES converter.cpp
            exceptions.cpp)
set(INCLUDES converter.h
             exceptions.h)

add_library(ConversionConverter STATIC)
target_sources(ConversionConverter
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(ConversionConverter
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(ConversionConverter
    PUBLIC      Conversion::Common
                Conversion::Registry
    PRIVATE     CommonProjectWarnings)

add_format_target(ConversionConverter)

add_library(Conversion::Converter ALIAS ConversionConverter)
