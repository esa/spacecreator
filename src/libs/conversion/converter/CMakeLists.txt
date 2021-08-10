project(ConversionConverter VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES model.cpp
            import/exceptions.cpp
            import/options.cpp)
set(INCLUDES model.h
             import/exceptions.h
             import/options.h)

add_library(ConversionConverter STATIC)
target_sources(ConversionConverter
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(ConversionConverter
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(ConversionConverter
    PUBLIC      Qt5::Core)

add_format_target(ConversionConverter)

add_library(Conversion::Converter ALIAS ConversionConverter)
