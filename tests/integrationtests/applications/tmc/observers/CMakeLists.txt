set(TEST_NAME tst_tmc_observers)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/tests)
target_link_libraries(${TEST_NAME}
    PRIVATE     Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test)

add_format_target(${TEST_NAME})

configure_file(test_input_monitoring.sh test_input_monitoring.sh COPYONLY)
configure_file(test_input_monitoring_failure.sh test_input_monitoring_failure.sh COPYONLY)
configure_file(test_msc_never_sequence.sh test_msc_never_sequence.sh COPYONLY)
configure_file(test_msc_never_sequence_failure.sh test_msc_never_sequence_failure.sh COPYONLY)
configure_file(test_msc_never_sequence_parameters_failure.sh test_msc_never_sequence_parameters_failure.sh COPYONLY)
configure_file(test_msc_never_sequence_backtracking.sh test_msc_never_sequence_backtracking.sh COPYONLY)
configure_file(test_msc_never_sequence_backtracking_failure.sh test_msc_never_sequence_backtracking_failure.sh COPYONLY)
configure_file(test_msc_when_then_sequence.sh test_msc_when_then_sequence.sh COPYONLY)
configure_file(test_msc_when_then_sequence_failure.sh test_msc_when_then_sequence_failure.sh COPYONLY)
configure_file(test_msc_when_then_not_sequence.sh test_msc_when_then_not_sequence.sh COPYONLY)
configure_file(test_msc_when_then_not_sequence_failure.sh test_msc_when_then_not_sequence_failure.sh COPYONLY)
configure_file(test_different_interface_names.sh test_different_interface_names.sh COPYONLY)
add_custom_target(${TEST_NAME}_copy_files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    DEPENDS ${TEST_NAME})
