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
    PRIVATE     ivcore
                libiveditor
    PUBLIC      Conversion::Common)

add_format_target(IvXmlExporter)

add_library(Conversion::IvXmlExporter ALIAS IvXmlExporter)
