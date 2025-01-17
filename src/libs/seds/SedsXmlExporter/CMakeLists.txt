project(SedsXmlExporter VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES     exporter.cpp
                specialized/datatypeexporter.cpp
                specialized/componentexporter.cpp
                specialized/interfaceexporter.cpp)

set(INCLUDES    exporter.h
                specialized/datatypeexporter.h
                specialized/componentexporter.h
                specialized/interfaceexporter.h)

add_library(SedsXmlExporter STATIC)
target_sources(SedsXmlExporter
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SedsXmlExporter
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SedsXmlExporter
    PUBLIC      Conversion::Common
                SEDS::Model
                Qt${QT_VERSION_MAJOR}::Xml
    PRIVATE     CommonProjectWarnings)

add_format_target(SedsXmlExporter)
add_tidy_target(SedsXmlExporter)

add_library(SEDS::XmlExporter ALIAS SedsXmlExporter)
