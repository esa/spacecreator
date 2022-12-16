
project(SdlModel VERSION 1.0.0 LANGUAGES CXX)

add_library(SdlModel STATIC)

set(SOURCES     action.cpp
                answer.cpp
                block.cpp
                channel.cpp
                connection.cpp
                expression.cpp
                decision.cpp
                input.cpp
                join.cpp
                label.cpp
                newtype.cpp
                nextstate.cpp
                node.cpp
                output.cpp
                procedure.cpp
                procedurecall.cpp
                procedureparameter.cpp
                process.cpp
                rename.cpp
                return.cpp
                route.cpp
                sdlmodel.cpp
                signal.cpp
                signalroute.cpp
                state.cpp
                statemachine.cpp
                syntype.cpp
                system.cpp
                task.cpp
                transition.cpp
                variabledeclaration.cpp
                variableliteral.cpp
                variablereference.cpp)
set(INCLUDES    action.h
                answer.h
                block.h
                channel.h
                connection.h
                expression.h
                decision.h
                input.h
                join.h
                label.h
                newtype.h
                nextstate.h
                node.h
                output.h
                procedure.h
                procedurecall.h
                procedureparameter.h
                process.h
                rename.h
                return.h
                route.h
                sdlmodel.h
                signal.h
                signalroute.h
                state.h
                statemachine.h
                syntype.h
                system.h
                task.h
                transition.h
                variabledeclaration.h
                variableliteral.h
                variablereference.h)

target_sources(SdlModel
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SdlModel
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SdlModel
    PUBLIC      Conversion::Common
    PRIVATE     CommonProjectWarnings)

add_format_target(SdlModel)
add_tidy_target(SdlModel)

add_library(SDL::Model ALIAS SdlModel)
