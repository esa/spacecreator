project(TestGenerator VERSION 1.0.0 LANGUAGES CXX)

add_library(TestGenerator STATIC)

target_sources(TestGenerator
    PRIVATE     datareconstructor/DataReconstructor.cpp
                datareconstructor/DataReconstructor.h
                gdbconnector/gdbconnector.cpp
                gdbconnector/gdbconnector.h
                gdbconnector/process.cpp
                gdbconnector/process.h
                testdrivergenerator/TestDriverGenerator.cpp
                testdrivergenerator/TestDriverGenerator.h
                testdrivergenerator/TestDriverGeneratorContext.cpp
                testdrivergenerator/TestDriverGeneratorContext.h
                testdrivergenerator/TestDriverGeneratorException.cpp
                testdrivergenerator/TestDriverGeneratorException.h
                harness/ivgenerator/ivgenerator.cpp
                harness/ivgenerator/ivgenerator.h
                harness/dvgenerator/dvgenerator.cpp
                harness/dvgenerator/dvgenerator.h
)

target_include_directories(TestGenerator
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs
)

target_link_libraries(TestGenerator
    PRIVATE     ivcore
                dvcore
                qobjectlistmodel
                CsvImporter
                CommonProjectWarnings
)

add_format_target(TestGenerator)
add_tidy_target(TestGenerator)
