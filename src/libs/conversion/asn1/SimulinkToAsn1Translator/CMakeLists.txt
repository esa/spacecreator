project(SimulinkToAsn1Translator VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES  translator.cpp
             datatypesdependencyresolver.cpp
             context.cpp
             specialized/datatypetranslatorvisitor.cpp
             specialized/busdatatypetranslator.cpp
             specialized/dimensiontranslator.cpp
             specialized/portdatatypetranslator.cpp)
set(INCLUDES translator.h
             datatypesdependencyresolver.h
             context.h
             specialized/datatypetranslatorvisitor.h
             specialized/busdatatypetranslator.h
             specialized/dimensiontranslator.h
             specialized/portdatatypetranslator.h)

add_library(SimulinkToAsn1Translator STATIC)
target_sources(SimulinkToAsn1Translator
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SimulinkToAsn1Translator
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SimulinkToAsn1Translator
    PUBLIC      Conversion::Common
                asn1library
                SIMULINK::Model
    PRIVATE     CommonProjectWarnings)

add_format_target(SimulinkToAsn1Translator)
add_tidy_target(SimulinkToAsn1Translator)

add_library(Conversion::SimulinkToAsn1Translator ALIAS SimulinkToAsn1Translator)
