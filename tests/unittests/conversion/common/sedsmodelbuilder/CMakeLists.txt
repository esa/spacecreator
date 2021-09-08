project(SedsModelBuilder VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES sedscomponentbuilder.cpp
            sedsinterfacebuilder.cpp
            sedsinterfacecommandbuilder.cpp
            sedsinterfacedeclarationbuilder.cpp
            sedsmodelbuilder.cpp)
set(INCLUDES sedscomponentbuilder.h
             sedsinterfacebuilder.h
             sedsinterfacecommandbuilder.h
             sedsinterfacedeclarationbuilder.h
             sedsmodelbuilder.h)

add_library(SedsModelBuilder STATIC)
target_sources(SedsModelBuilder
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SedsModelBuilder
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SedsModelBuilder
    PUBLIC     SEDS::Model)

add_format_target(SedsModelBuilder)

add_library(ConversionTests::SedsModelBuilder ALIAS SedsModelBuilder)
