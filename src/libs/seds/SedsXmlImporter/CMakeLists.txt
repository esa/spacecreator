project(SedsXmlImporter VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES importer.cpp)

set(INCLUDES importer.h)

add_library(SedsXmlImporter STATIC)
target_sources(SedsXmlImporter
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SedsXmlImporter
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SedsXmlImporter
    PUBLIC      Conversion::Common
                SEDS::SymbolDefinitionReader
    PRIVATE     SEDS::XmlParser
                SEDS::XmlPreprocessor
                SEDS::XmlValidator
                Qt5::Xml
                CommonProjectWarnings)

add_format_target(SedsXmlImporter)

add_library(SEDS::XmlImporter ALIAS SedsXmlImporter)
