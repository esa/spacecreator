project(SedsXmlExporter VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES exporter.cpp)

set(INCLUDES exporter.h)

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
                Qt5::Xml
    PRIVATE     CommonProjectWarnings)

add_format_target(SedsXmlExporter)
add_tidy_target(SedsXmlExporter)

add_library(SEDS::XmlExporter ALIAS SedsXmlExporter)
