set(TEST_NAME tsti_promelaexporter)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(${TEST_NAME}
    PRIVATE     Conversion::Common
                Conversion::PromelaExporter
                Qt5::Core
                Qt5::Test)

configure_file(resources/expect_promela_file1.pml expect_promela_file1.pml COPYONLY)
configure_file(resources/expect_promela_file2.pml expect_promela_file2.pml COPYONLY)
configure_file(resources/expect_promela_file3.pml expect_promela_file3.pml COPYONLY)

add_format_target(${TEST_NAME})
