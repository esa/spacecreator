project(SedsToIvTranslator VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES     context.cpp
                interfacetranslatorhelper.cpp
                interfacetypenamehelper.cpp
                translator.cpp
                specialized/asyncinterfacecommandtranslator.cpp
                specialized/componentstranslator.cpp
                specialized/descriptiontranslator.cpp
                specialized/devicetranslator.cpp
                specialized/interfaceparametertranslator.cpp
                specialized/syncinterfacecommandtranslator.cpp)
set(INCLUDES    context.h
                interfacetranslatorhelper.h
                interfacetypenamehelper.h
                translator.h
                specialized/asyncinterfacecommandtranslator.h
                specialized/componentstranslator.h
                specialized/descriptiontranslator.h
                specialized/devicetranslator.h
                specialized/interfaceparametertranslator.h
                specialized/syncinterfacecommandtranslator.h)

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
    PRIVATE     CommonProjectWarnings
                Conversion::SedsToAsn1Translator)

add_format_target(SedsToIvTranslator)
add_tidy_target(SedsToIvTranslator)

add_library(Conversion::SedsToIvTranslator ALIAS SedsToIvTranslator)
