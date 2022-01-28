project(TestGenerator VERSION 1.0.0 LANGUAGES CXX)

add_library(TestGenerator STATIC)

target_sources(TestGenerator
    PRIVATE     TestGenerator.cpp
                TestGenerator.h
                TestGeneratorContext.cpp
                TestGeneratorContext.h
                TestGeneratorException.cpp
                TestGeneratorException.h
)

target_include_directories(TestGenerator
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs
)

target_link_libraries(TestGenerator
    PRIVATE     ivcore
                CsvImporter
                CommonProjectWarnings
)

add_format_target(TestGenerator)
add_tidy_target(TestGenerator)
