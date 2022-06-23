project(SimulinkXmlImporter VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
    importer.cpp
    exceptions.cpp)

set(INCLUDES
    importer.h
    exceptions.h)

add_library(SimulinkXmlImporter STATIC)
target_sources(SimulinkXmlImporter
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SimulinkXmlImporter
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SimulinkXmlImporter
    PUBLIC      Conversion::Common
                SIMULINK::Model
    PRIVATE     Qt${QT_VERSION_MAJOR}::Xml
                CommonProjectWarnings)

add_format_target(SimulinkXmlImporter)
add_tidy_target(SimulinkXmlImporter)

add_library(SIMULINK::XmlImporter ALIAS SimulinkXmlImporter)
