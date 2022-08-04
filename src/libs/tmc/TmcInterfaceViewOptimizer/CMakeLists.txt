project(TmcInterfaceViewOptimizer VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES interfaceviewoptimizer.cpp)
set(INCLUDES interfaceviewoptimizer.h)

add_library(TmcInterfaceViewOptimizer STATIC)
target_sources(TmcInterfaceViewOptimizer
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(TmcInterfaceViewOptimizer
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(TmcInterfaceViewOptimizer
    PUBLIC      ivcore
                libiveditor
    PRIVATE     CommonProjectWarnings)

add_format_target(TmcInterfaceViewOptimizer)
add_tidy_target(TmcInterfaceViewOptimizer)

add_library(Tmc::InterfaceViewOptimizer ALIAS TmcInterfaceViewOptimizer)
