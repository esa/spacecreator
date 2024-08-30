set(TEST_NAME tst_tmc_types)
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

configure_file(test_bool.sh test_bool.sh COPYONLY)
configure_file(test_choice.sh test_choice.sh COPYONLY)
configure_file(test_enum.sh test_enum.sh COPYONLY)
configure_file(test_ia5string.sh test_ia5string.sh COPYONLY)
configure_file(test_integer.sh test_integer.sh COPYONLY)
configure_file(test_real.sh test_real.sh COPYONLY)
configure_file(test_real_constraints.sh test_real_constraints.sh COPYONLY)
configure_file(test_real_generation.sh test_real_generation.sh COPYONLY)
configure_file(test_octet.sh test_octet.sh COPYONLY)
configure_file(test_sequence.sh test_sequence.sh COPYONLY)
configure_file(test_sequenceof.sh test_sequenceof.sh COPYONLY)
configure_file(test_choice_name_mangling.sh test_choice_name_mangling.sh COPYONLY)
configure_file(test_nested_sequenceof.sh test_nested_sequenceof.sh COPYONLY)
add_custom_target(${TEST_NAME}_copy_files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    DEPENDS ${TEST_NAME})
