set(TEST_NAME tst_functiontesterplugin)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp
)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs
                ${CMAKE_SOURCE_DIR}/tests/unittests
)
target_link_libraries(${TEST_NAME}
    PRIVATE     FunctionTesterPlugin
                Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test
                CommonProjectWarnings
)

configure_file(resources/project/test.csv resources/project/test.csv COPYONLY)
configure_file(resources/project/interfaceview.xml resources/project/interfaceview.xml COPYONLY)
configure_file(resources/project/dataview-uniq.asn resources/project/dataview-uniq.asn COPYONLY)
configure_file(resources/generated/testdriver.c resources/generated/testdriver.c COPYONLY)
configure_file(resources/generated/interfaceview.xml resources/generated/interfaceview.xml COPYONLY)
configure_file(resources/generated/deploymentview.dv.xml resources/generated/deploymentview.dv.xml COPYONLY)

add_format_target(${TEST_NAME})
