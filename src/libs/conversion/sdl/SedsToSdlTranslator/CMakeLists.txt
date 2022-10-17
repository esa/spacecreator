project(SedsToSdlTranslator VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES translator.cpp
            specialized/activitytranslator.cpp
            specialized/descriptiontranslator.cpp
            specialized/common.cpp
            specialized/mathoperationtranslator.cpp
            specialized/splinecalibratortranslator.cpp
            specialized/statemachinetranslator.cpp
            specialized/statementtranslatorvisitor.cpp)
set(INCLUDES constants.h
             translator.h
             specialized/activitytranslator.h
             specialized/common.h
             specialized/descriptiontranslator.h
             specialized/mathoperationtranslator.h
             specialized/splinecalibratortranslator.h
             specialized/statemachinetranslator.h
             specialized/statementtranslatorvisitor.h)

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
                SdlModel
    PRIVATE     CommonProjectWarnings)

add_format_target(SedsToSdlTranslator)
add_tidy_target(SedsToSdlTranslator)

add_library(Conversion::SedsToSdlTranslator ALIAS SedsToSdlTranslator)
