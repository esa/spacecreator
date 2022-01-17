set(TEST_NAME tst_csvimporter)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs
                ${CMAKE_SOURCE_DIR}/tests/unittests)
target_link_libraries(${TEST_NAME}
    PRIVATE     CsvImporter
                CsvModel
                Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test
                CommonProjectWarnings)

configure_file(resources/empty.csv resources/empty.csv COPYONLY)
configure_file(resources/header_only.csv resources/header_only.csv COPYONLY)
configure_file(resources/one_row_with_header.csv resources/one_row_with_header.csv COPYONLY)
configure_file(resources/one_row_no_header.csv resources/one_row_no_header.csv COPYONLY)
configure_file(resources/two_rows_with_header.csv resources/two_rows_with_header.csv COPYONLY)
configure_file(resources/two_rows_no_header.csv resources/two_rows_no_header.csv COPYONLY)
configure_file(resources/many_rows_with_header.csv resources/many_rows_with_header.csv COPYONLY)
configure_file(resources/many_rows_no_header.csv resources/many_rows_no_header.csv COPYONLY)

add_format_target(${TEST_NAME})
