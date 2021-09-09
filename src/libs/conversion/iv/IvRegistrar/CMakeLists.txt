project(IvRegistrar VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES registrar.cpp)
set(INCLUDES registrar.h)

add_library(IvRegistrar STATIC)
target_sources(IvRegistrar
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(IvRegistrar
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(IvRegistrar
    PUBLIC      Conversion::Common
                Conversion::IvXmlImporter
                Conversion::IvXmlExporter
                Conversion::SedsToIvTranslator
    PRIVATE     CommonProjectWarnings)

add_format_target(IvRegistrar)

add_library(Conversion::IvRegistrar ALIAS IvRegistrar)
