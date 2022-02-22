project(SedsModelBuilder VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES sedsactivitybuilder.cpp
            sedsalternatebuilder.cpp
            sedsalternatesetbuilder.cpp
            sedscommandprimitivebuilder.cpp
            sedscomponentbuilder.cpp
            sedscontainerdatatypebuilder.cpp
            sedsdatatypefactory.cpp
            sedsimplementationbuilder.cpp
            sedsinterfacebuilder.cpp
            sedsinterfacecommandbuilder.cpp
            sedsinterfacedeclarationbuilder.cpp
            sedsinvocationbuilder.cpp
            sedsmodelbuilder.cpp
            sedsstatemachinebuilder.cpp
            sedstypemapsetbuilder.cpp)
set(INCLUDES sedsactivitybuilder.h
             sedsalternatebuilder.h
             sedsalternatesetbuilder.h
             sedscommandprimitivebuilder.h
             sedscomponentbuilder.h
             sedscontainerdatatypebuilder.h
             sedsdatatypefactory.h
             sedsimplementationbuilder.h
             sedsinterfacebuilder.h
             sedsinterfacecommandbuilder.h
             sedsinterfacedeclarationbuilder.h
             sedsinvocationbuilder.h
             sedsmodelbuilder.h
             sedsstatemachinebuilder.h
             sedstypemapsetbuilder.h)

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
