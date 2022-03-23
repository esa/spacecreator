set(TEST_NAME tst_datareconstructor)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp
                gdbconnectorstub.cpp
                gdbconnectorstub.h
)

target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs
                ${CMAKE_SOURCE_DIR}/tests/unittests
)

target_link_libraries(${TEST_NAME}
    PRIVATE     Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test
                TestGenerator
                CommonProjectWarnings
)

add_format_target(${TEST_NAME})
