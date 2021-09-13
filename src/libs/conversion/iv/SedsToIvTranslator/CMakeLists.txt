project(SedsToIvTranslator VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES translator.cpp
            specialized/asyncinterfacecommandtranslator.cpp
            specialized/interfacecommandtranslator.cpp
            specialized/componentstranslator.cpp)
set(INCLUDES translator.h
             specialized/asyncinterfacecommandtranslator.h
             specialized/interfacecommandtranslator.h
             specialized/componentstranslator.h)

add_library(SedsToIvTranslator STATIC)
target_sources(SedsToIvTranslator
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SedsToIvTranslator
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SedsToIvTranslator
    PUBLIC      Conversion::Common
                ivcore
                asn1library
                SEDS::Model
                Conversion::SedsToAsn1Translator
    PRIVATE     CommonProjectWarnings)

add_format_target(SedsToIvTranslator)
add_tidy_target(SedsToIvTranslator)

add_library(Conversion::SedsToIvTranslator ALIAS SedsToIvTranslator)
