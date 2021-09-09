project(SedsToAsn1Translator VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES translator.cpp
            visitors/datatypetranslatorvisitor.cpp
            visitors/floatrangetranslatorvisitor.cpp
            visitors/integerrangetranslatorvisitor.cpp)
set(INCLUDES translator.h
             visitors/datatypetranslatorvisitor.h
             visitors/floatrangetranslatorvisitor.h
             visitors/integerrangetranslatorvisitor.h)

add_library(SedsToAsn1Translator STATIC)
target_sources(SedsToAsn1Translator
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SedsToAsn1Translator
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SedsToAsn1Translator
    PUBLIC      Conversion::Common
                asn1library
                SEDS::Model
    PRIVATE     CommonProjectWarnings)

add_format_target(SedsToAsn1Translator)

add_library(Conversion::SedsToAsn1Translator ALIAS SedsToAsn1Translator)
