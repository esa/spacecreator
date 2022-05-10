project(SedsToAsn1Translator VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES     context.cpp
                datatypetranslationhelper.cpp
                datatypesdependencyresolver.cpp
                generictypecreator.cpp
                generictypemapper.cpp
                interfacetypecreator.cpp
                interfacetypecreatorcontext.cpp
                packagesdependencyresolver.cpp
                patchersnippetsgenerator.cpp
                translator.cpp
                specialized/arraydatatypetranslator.cpp
                specialized/containerconstrainttranslatorvisitor.cpp
                specialized/containerdatatypetranslator.cpp
                specialized/datatypetranslatorvisitor.cpp
                specialized/descriptiontranslator.cpp
                specialized/dimensiontranslator.cpp
                specialized/entrytranslatorvisitor.cpp
                specialized/genericinterfacetypecreator.cpp
                specialized/interfacedeclarationtypecreator.cpp
                specialized/rangetranslatorvisitor.cpp)
set(INCLUDES    context.h
                datatypetranslationhelper.h
                datatypesdependencyresolver.h
                generictypecreator.cpp
                generictypemapper.h
                interfacetypecreator.h
                interfacetypecreatorcontext.h
                packagesdependencyresolver.h
                patchersnippetsgenerator.h
                translator.h
                specialized/arraydatatypetranslator.h
                specialized/containerconstrainttranslatorvisitor.h
                specialized/containerdatatypetranslator.h
                specialized/datatypetranslatorvisitor.h
                specialized/descriptiontranslator.h
                specialized/dimensiontranslator.h
                specialized/entrytranslatorvisitor.h
                specialized/genericinterfacetypecreator.h
                specialized/interfacedeclarationtypecreator.h
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
