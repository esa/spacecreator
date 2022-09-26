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
                CommonTools
                CommonProjectWarnings
)

add_format_target(${TEST_NAME})

configure_file(resources/config.xml resources/config.xml COPYONLY)
configure_file(resources/interfaceview.xml resources/interfaceview.xml COPYONLY)
configure_file(resources/interfaceview_big_endian.xml resources/interfaceview_big_endian.xml COPYONLY)
configure_file(resources/testharness.asn resources/testharness.asn COPYONLY)
configure_file(resources/testharness_big_endian.asn resources/testharness_big_endian.asn COPYONLY)
configure_file(resources/interfaceview_booleans.xml resources/interfaceview_booleans.xml COPYONLY)
