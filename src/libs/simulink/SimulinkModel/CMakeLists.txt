project(SimulinkModel VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
    base/namedentity.cpp
    simulinkmodel.cpp
    modelinterface.cpp
    inport/inport.cpp
    outport/outport.cpp
    datatypes/aliasdatatype.cpp
    datatypes/busdatatype.cpp
    datatypes/enumdatatype.cpp
    datatypes/busmember.cpp
    datatypes/enumvalue.cpp
    datatypes/datatype.cpp
    datatypes/rootdatatype.cpp
    )
set(INCLUDES
    base/namedentity.h
    simulinkmodel.h
    modelinterface.h
    inport/inport.h
    outport/outport.h
    datatypes/aliasdatatype.h
    datatypes/busdatatype.h
    datatypes/enumdatatype.h
    datatypes/busmember.h
    datatypes/enumvalue.h
    datatypes/datatype.h
    datatypes/rootdatatype.h
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
