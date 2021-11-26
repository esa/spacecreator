set(TEST_NAME tst_sedsconverter_asn1toseds)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/tests)
target_link_libraries(${TEST_NAME}
    PRIVATE     Qt5::Core
                Qt5::Test)

add_format_target(${TEST_NAME})

configure_file(test_simple_data_types.sh test_simple_data_types.sh COPYONLY)
configure_file(resources/test_simple_data_types.asn resources/test_simple_data_types.asn COPYONLY)
