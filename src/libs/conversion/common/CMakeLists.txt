project(ConversionCommon VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES exceptions.cpp
            modeltype.cpp
            options.cpp
            export/exceptions.cpp
            import/exceptions.cpp
            translation/exceptions.cpp)
set(INCLUDES exceptions.h
             model.h
             modeltype.h
             options.h
             registrar.h
             export/exceptions.h
             export/modelexporter.h
             import/exceptions.h
             import/modelimporter.h
             translation/exceptions.h
             translation/translator.h)

add_library(ConversionCommon STATIC)
target_sources(ConversionCommon
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(ConversionCommon
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(ConversionCommon
    PUBLIC      Qt5::Core
    PRIVATE     CommonProjectWarnings)

add_format_target(ConversionCommon)

add_library(Conversion::Common ALIAS ConversionCommon)
