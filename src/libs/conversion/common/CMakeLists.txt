project(ConversionCommon VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES exceptions.cpp
            modeltype.cpp
            options.cpp
            escaper/escaper.cpp
            escaper/exceptions.cpp
            export/exceptions.cpp
            export/modelexporter.cpp
            import/exceptions.cpp
            translation/exceptions.cpp
            translation/translator.cpp)
set(INCLUDES exceptions.h
             model.h
             modelproperties.h
             modeltype.h
             options.h
             overloaded.h
             registrar.h
             escaper/escaper.h
             escaper/exceptions.h
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
    PUBLIC      Qt${QT_VERSION_MAJOR}::Core
    PRIVATE     CommonProjectWarnings)

add_format_target(ConversionCommon)
add_tidy_target(ConversionCommon)

add_library(Conversion::Common ALIAS ConversionCommon)
