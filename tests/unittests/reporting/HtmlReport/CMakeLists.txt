set(TEST_NAME tst_htmlreportbuilder)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs
                ${CMAKE_SOURCE_DIR}/tests)
target_link_libraries(${TEST_NAME}
    PRIVATE     HtmlReport
                Report
                Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test)

configure_file(resources/empty_message.txt resources/empty_message.txt COPYONLY)
configure_file(resources/result_empty.html resources/result_empty.html COPYONLY)
configure_file(resources/result_error.html resources/result_error.html COPYONLY)
configure_file(resources/spin_error_output.txt resources/spin_error_output.txt COPYONLY)
configure_file(resources/template.html resources/template.html COPYONLY)

add_format_target(${TEST_NAME})
