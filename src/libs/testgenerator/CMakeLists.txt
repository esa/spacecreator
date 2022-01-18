project(TestGenerator VERSION 1.0.0 LANGUAGES CXX)

add_library(TestGenerator STATIC)

target_sources(TestGenerator
    PRIVATE     TestGenerator.cpp
                TestGenerator.h
)

target_include_directories(TestGenerator
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs
)

target_link_libraries(TestGenerator
    PRIVATE     CommonProjectWarnings
                CsvImporter
)

add_format_target(TestGenerator)
add_tidy_target(TestGenerator)
