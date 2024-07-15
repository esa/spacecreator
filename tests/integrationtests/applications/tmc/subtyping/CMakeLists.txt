set(TEST_NAME tst_tmc_subtyping)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/tests)
target_link_libraries(${TEST_NAME}
    PRIVATE     Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test
                CommonProjectWarnings)

add_format_target(${TEST_NAME})

configure_file(test_external_proc.sh test_external_proc.sh COPYONLY)
configure_file(test_subtyping.sh test_subtyping.sh COPYONLY)
configure_file(test_same_size.sh test_same_size.sh COPYONLY)
configure_file(test_sequenceof_fixed_size.sh test_sequenceof_fixed_size.sh COPYONLY)
configure_file(test_ia5string_fixed_size.sh test_ia5string_fixed_size.sh COPYONLY)
configure_file(test_octetstring_fixed_size.sh test_octetstring_fixed_size.sh COPYONLY)
configure_file(test_octetstring.sh test_octetstring.sh COPYONLY)
configure_file(test_octetstring_with_sequenceof.sh test_octetstring_with_sequenceof.sh COPYONLY)
configure_file(test_name_matching.sh test_name_matching.sh COPYONLY)
add_custom_target(${TEST_NAME}_copy_files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    DEPENDS ${TEST_NAME})
