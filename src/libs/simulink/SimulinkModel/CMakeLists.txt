project(SimulinkModel VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
    simulinkmodel.cpp
    modelinterface.cpp
    inport/inports.cpp
    inport/inport.cpp
    outport/outports.cpp
    outport/outport.cpp
    workspace/member.cpp
    workspace/memberset.cpp
    workspace/workspace.cpp
    workspace/workspaceelement.cpp
    )
set(INCLUDES
    simulinkmodel.h
    modelinterface.h
    inport/inports.h
    inport/inport.h
    outport/outports.h
    outport/outport.h
    workspace/member.h
    workspace/memberset.h
    workspace/workspace.h
    workspace/workspaceelement.h
    )

add_library(SimulinkModel STATIC)
target_sources(SimulinkModel
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SimulinkModel
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SimulinkModel
    PUBLIC     Conversion::Common
               shared
    PRIVATE    CommonProjectWarnings)

add_format_target(SimulinkModel)
add_tidy_target(SimulinkModel)

add_library(SIMULINK::Model ALIAS SimulinkModel)
