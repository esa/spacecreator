project(tmcivoptimizer VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES main.cpp)

set(INCLUDES )

add_executable(tmcivoptimizer)
target_sources(tmcivoptimizer
    PRIVATE    "${SOURCES}"
               "${INCLUDES}")
target_include_directories(tmcivoptimizer
    PRIVATE    .
               ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(tmcivoptimizer
    PRIVATE    Tmc::InterfaceViewOptimizer
               CommonProjectWarnings)

add_format_target(tmcivoptimizer)

set_target_properties(tmcivoptimizer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    OUTPUT_NAME "tmcivoptimizer"
)

install(TARGETS tmcivoptimizer DESTINATION bin)
