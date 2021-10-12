project(SdlModelBuilder VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES sdlmodelbuilder.cpp
            sdlprocessbuilder.cpp)
set(INCLUDES sdlmodelbuilder.h
             sdlprocessbuilder.h)

add_library(SdlModelBuilder STATIC)
target_sources(SdlModelBuilder
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SdlModelBuilder
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SdlModelBuilder
    PUBLIC      SdlModel
    PRIVATE     CommonProjectWarnings)

add_format_target(SdlModelBuilder)

add_library(ConversionTests::SdlModelBuilder ALIAS SdlModelBuilder)
