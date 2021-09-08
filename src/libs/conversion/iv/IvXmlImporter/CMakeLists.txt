project(IvXmlImporter VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES importer.cpp)

set(INCLUDES importer.h)

add_library(IvXmlImporter STATIC)
target_sources(IvXmlImporter
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(IvXmlImporter
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(IvXmlImporter
    PRIVATE     ivcore
    PUBLIC      Conversion::Common)

add_format_target(IvXmlImporter)

add_library(Conversion::IvXmlImporter ALIAS IvXmlImporter)
