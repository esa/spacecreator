project(SimulinkToIvTranslator VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES     translator.cpp
                specialized/portstranslator.cpp)
set(INCLUDES    translator.h
                options.h
                specialized/portstranslator.h)

add_library(SimulinkToIvTranslator STATIC)
target_sources(SimulinkToIvTranslator
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SimulinkToIvTranslator
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SimulinkToIvTranslator
    PUBLIC      Conversion::Common
                ivcore
                SIMULINK::Model
    PRIVATE     CommonProjectWarnings)

add_format_target(SimulinkToIvTranslator)
add_tidy_target(SimulinkToIvTranslator)

add_library(Conversion::SimulinkToIvTranslator ALIAS SimulinkToIvTranslator)
