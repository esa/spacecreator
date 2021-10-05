
project(SdlModel VERSION 1.0.0 LANGUAGES CXX)

add_library(SdlModel STATIC)

set(SOURCES action.cpp
            input.cpp
            literal.cpp
            nextstate.cpp
            node.cpp
            process.cpp
            sdlmodel.cpp
            signal.cpp
            state.cpp
            statemachine.cpp
            transition.cpp
            variabledeclaration.cpp
            variablereference.cpp)
set(INCLUDES sdlmodel.h)

target_sources(SdlModel
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SdlModel
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/sdlmodel
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SdlModel
    PUBLIC      Conversion::Common
                CommonProjectWarnings)

add_format_target(SdlModel)
add_tidy_target(SdlModel)

add_library(Conversion::SdlModel ALIAS SdlModel)
