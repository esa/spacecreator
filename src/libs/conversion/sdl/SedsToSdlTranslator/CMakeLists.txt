project(SedsToSdlTranslator VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES translator.cpp
            specialized/statemachinetranslator.cpp
            specialized/activitytranslator.cpp)
set(INCLUDES translator.h
             specialized/statemachinetranslator.h
             specialized/activitytranslator.h)

add_library(SedsToSdlTranslator STATIC)
target_sources(SedsToSdlTranslator
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SedsToSdlTranslator
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SedsToSdlTranslator
    PUBLIC      Conversion::Common
                SedsToIvTranslator
                asn1library
                ivcore
                SEDS::Model
    PRIVATE     CommonProjectWarnings)

add_format_target(SedsToSdlTranslator)
add_tidy_target(SedsToSdlTranslator)

add_library(Conversion::SedsToSdlTranslator ALIAS SedsToSdlTranslator)
