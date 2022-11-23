project(trailgen VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES main.cpp
    executor.cpp)

set(INCLUDES executor.h)

add_executable(trailgen)
target_sources(trailgen
    PRIVATE    "${SOURCES}"
               "${INCLUDES}")
target_include_directories(trailgen
    PRIVATE    .
               ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(trailgen
    PRIVATE    Conversion::Converter
               Conversion::Common
               Tmc::Converter
               shared
               CommonProjectWarnings)

add_format_target(trailgen)

set_target_properties(trailgen PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    OUTPUT_NAME "trailgen"
)

install(TARGETS trailgen DESTINATION bin)
