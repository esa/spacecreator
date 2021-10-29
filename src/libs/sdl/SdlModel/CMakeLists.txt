
project(SdlModel VERSION 1.0.0 LANGUAGES CXX)

add_library(SdlModel STATIC)

set(SOURCES action.cpp
            answer.cpp
            expression.cpp
            decision.cpp
            input.cpp
            join.cpp
            label.cpp
            nextstate.cpp
            node.cpp
            output.cpp
            procedure.cpp
            procedurecall.cpp
            procedureparameter.cpp
            process.cpp
            sdlmodel.cpp
            signal.cpp
            state.cpp
            statemachine.cpp
            task.cpp
            transition.cpp
            variabledeclaration.cpp
            variableliteral.cpp
            variablereference.cpp
            )
set(INCLUDES action.h
             answer.h
             expression.h
             decision.h
             input.h
             join.h
             label.h
             nextstate.h
             node.h
             output.h
             procedure.h
             procedurecall.h
             procedureparameter.h
             process.h
             sdlmodel.h
             signal.h
             state.h
             statemachine.h
             task.h
             transition.h
             variabledeclaration.h
             variableliteral.h
             variablereference.h
             )

target_sources(SdlModel
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SdlModel
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SdlModel
    PUBLIC      Conversion::Common
                CommonProjectWarnings)

add_format_target(SdlModel)
add_tidy_target(SdlModel)

add_library(Conversion::SdlModel ALIAS SdlModel)
