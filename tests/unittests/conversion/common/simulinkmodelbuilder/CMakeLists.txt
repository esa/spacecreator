project(SimulinkModelBuilder VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES  simulinkmodelbuilder.cpp
             simulinkbusdatatypebuilder.cpp)
set(INCLUDES simulinkmodelbuilder.h
             simulinkbusdatatypebuilder.h)

add_library(SimulinkModelBuilder STATIC)
target_sources(SimulinkModelBuilder
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SimulinkModelBuilder
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SimulinkModelBuilder
    PUBLIC      SIMULINK::Model
    PRIVATE     CommonProjectWarnings)

add_format_target(SimulinkModelBuilder)

add_library(ConversionTests::SimulinkModelBuilder ALIAS SimulinkModelBuilder)
