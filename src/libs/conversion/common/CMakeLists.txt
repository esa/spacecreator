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
            translation/translator.cpp
            simulink/matlabstandarddatatypes.cpp
            simulink/tastestandarddatatypes.cpp
            simulink/portsdatatypes.cpp)
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
             translation/translator.h
             simulink/matlabstandarddatatypes.h
             simulink/tastestandarddatatypes.h
             simulink/portsdatatypes.h)

add_library(ConversionCommon STATIC)
target_sources(ConversionCommon
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(ConversionCommon
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(ConversionCommon
    PUBLIC      Qt${QT_VERSION_MAJOR}::Core
                asn1library
                shared
    PRIVATE     CommonProjectWarnings)

add_format_target(ConversionCommon)
add_tidy_target(ConversionCommon)

add_library(Conversion::Common ALIAS ConversionCommon)
