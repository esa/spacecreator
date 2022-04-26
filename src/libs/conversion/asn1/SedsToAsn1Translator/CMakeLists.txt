project(SedsToAsn1Translator VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES     context.cpp
                datatypesdependencyresolver.cpp
                packagesdependencyresolver.cpp
                patchersnippetsgenerator.cpp
                translator.cpp
                specialized/arraydatatypetranslator.cpp
                specialized/datatypetranslatorvisitor.cpp
                specialized/descriptiontranslator.cpp
                specialized/dimensiontranslator.cpp
                specialized/rangetranslatorvisitor.cpp)
set(INCLUDES    context.h
                datatypesdependencyresolver.h
                packagesdependencyresolver.h
                patchersnippetsgenerator.h
                translator.h
                specialized/arraydatatypetranslator.h
                specialized/datatypetranslatorvisitor.h
                specialized/descriptiontranslator.h
                specialized/dimensiontranslator.h
                specialized/rangetranslatorvisitor.h
                specialized/sizetranslatorvisitor.h)

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
add_tidy_target(SedsToAsn1Translator)

add_library(Conversion::SedsToAsn1Translator ALIAS SedsToAsn1Translator)
