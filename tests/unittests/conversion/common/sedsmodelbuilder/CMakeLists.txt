project(SedsModelBuilder VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES sedscomponentbuilder.cpp
            sedscontainerdatatypebuilder.cpp
            sedsdatatypefactory.cpp
            sedsinterfacebuilder.cpp
            sedsinterfacecommandbuilder.cpp
            sedsinterfacedeclarationbuilder.cpp
            sedsmodelbuilder.cpp
            sedsimplementationbuilder.cpp
            sedsstatemachinebuilder.cpp
            sedsactivitybuilder.cpp
            sedscommandprimitivebuilder.cpp
            sedsinvocationbuilder.cpp)
set(INCLUDES sedscomponentbuilder.h
             sedscontainerdatatypebuilder.h
             sedsdatatypefactory.h
             sedsinterfacebuilder.h
             sedsinterfacecommandbuilder.h
             sedsinterfacedeclarationbuilder.h
             sedsmodelbuilder.h
             sedsimplementationbuilder.h
             sedsstatemachinebuilder.h
             sedsactivitybuilder.h
             sedscommandprimitivebuilder.h
             sedsinvocationbuilder.h)

add_library(SedsModelBuilder STATIC)
target_sources(SedsModelBuilder
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SedsModelBuilder
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SedsModelBuilder
    PUBLIC      SEDS::Model
    PRIVATE     CommonProjectWarnings)

add_format_target(SedsModelBuilder)

add_library(ConversionTests::SedsModelBuilder ALIAS SedsModelBuilder)
