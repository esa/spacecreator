project(IvXmlExporter VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES exporter.cpp)
set(INCLUDES exporter.h)

add_library(IvXmlExporter STATIC)
target_sources(IvXmlExporter
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(IvXmlExporter
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(IvXmlExporter
    PUBLIC      Conversion::Common
    PRIVATE     ivcore
                libiveditor
                CommonProjectWarnings)

add_format_target(IvXmlExporter)
add_tidy_target(IvXmlExporter)

add_library(IV::XmlExporter ALIAS IvXmlExporter)
