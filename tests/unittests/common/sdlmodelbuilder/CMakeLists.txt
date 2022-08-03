project(SdlModelBuilder VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES     sdlanswerbuilder.cpp
                sdlblockbuilder.cpp
                sdlchannelbuilder.cpp
                sdldecisionbuilder.cpp
                sdlmodelbuilder.cpp
                sdlinputbuilder.cpp
                sdloutputbuilder.cpp
                sdlprocedurebuilder.cpp
                sdlprocedurecallbuilder.cpp
                sdlprocessbuilder.cpp
                sdlsignalroutebuilder.cpp
                sdlstatebuilder.cpp
                sdlstatemachinebuilder.cpp
                sdlsystembuilder.cpp
                sdltaskbuilder.cpp
                sdltransitionbuilder.cpp)
set(INCLUDES    sdlanswerbuilder.h
                sdlblockbuilder.h
                sdlchannelbuilder.h
                sdldecisionbuilder.h
                sdlmodelbuilder.h
                sdlinputbuilder.h
                sdloutputbuilder.cpp
                sdlprocedurebuilder.h
                sdlprocedurecallbuilder.h
                sdlprocessbuilder.h
                sdlsignalroutebuilder.h
                sdlstatebuilder.h
                sdlstatemachinebuilder.h
                sdlsystembuilder.h
                sdltaskbuilder.h
                sdltransitionbuilder.h)

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
add_tidy_target(SdlModelBuilder)

add_library(ConversionTests::SdlModelBuilder ALIAS SdlModelBuilder)
