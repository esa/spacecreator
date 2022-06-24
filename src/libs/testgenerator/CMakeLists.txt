project(TestGenerator VERSION 1.0.0 LANGUAGES CXX)

add_library(TestGenerator STATIC)

target_sources(TestGenerator
    PRIVATE     testgenerator.cpp
                datareconstructor/datareconstructor.cpp
                datareconstructor/datareconstructor.h
                gdbconnector/gdbconnector.cpp
                gdbconnector/gdbconnector.h
                gdbconnector/process.cpp
                gdbconnector/process.h
                testdrivergenerator/testdrivergenerator.cpp
                testdrivergenerator/testdrivergenerator.h
                testdrivergenerator/testdrivergeneratorcontext.cpp
                testdrivergenerator/testdrivergeneratorcontext.h
                testdrivergenerator/testdrivergeneratorexception.cpp
                testdrivergenerator/testdrivergeneratorexception.h
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
                libiveditor
                libdveditor
                qobjectlistmodel
                CsvImporter
                CommonProjectWarnings
                CommonTools
)

add_format_target(TestGenerator)
add_tidy_target(TestGenerator)
