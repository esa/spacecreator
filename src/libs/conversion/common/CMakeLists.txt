project(ConversionCommon VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES exceptions.cpp
            model.cpp
            modeltype.cpp
            options.cpp
            import/exceptions.cpp)
set(INCLUDES exceptions.h
             model.h
             modeltype.h
             options.h
             registrar.h
             import/exceptions.h
             import/modelimporter.h)

add_library(ConversionCommon STATIC)
target_sources(ConversionCommon
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(ConversionCommon
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(ConversionCommon
    PUBLIC      Qt5::Core)

add_format_target(ConversionCommon)

add_library(Conversion::Common ALIAS ConversionCommon)
