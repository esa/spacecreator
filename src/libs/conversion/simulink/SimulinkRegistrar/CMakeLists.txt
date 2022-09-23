project(SimulinkRegistrar VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES registrar.cpp)
set(INCLUDES registrar.h)

add_library(SimulinkRegistrar STATIC)
target_sources(SimulinkRegistrar
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SimulinkRegistrar
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SimulinkRegistrar
    PRIVATE     SIMULINK::XmlImporter
                CommonProjectWarnings
    PUBLIC      Conversion::Common)

add_format_target(SimulinkRegistrar)
add_tidy_target(SimulinkRegistrar)

add_library(Conversion::SimulinkRegistrar ALIAS SimulinkRegistrar)
