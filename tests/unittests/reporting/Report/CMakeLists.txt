set(TEST_NAME tst_spinerrorparser)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs
                ${CMAKE_SOURCE_DIR}/tests)
target_link_libraries(${TEST_NAME}
    PRIVATE     Report
                Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test)

configure_file(resources/obs_failure_error_state_msg.txt resources/obs_failure_error_state_msg.txt COPYONLY)
configure_file(resources/obs_failure_error_state_spin.txt resources/obs_failure_error_state_spin.txt COPYONLY)
configure_file(resources/obs_failure_error_state_scl.txt resources/obs_failure_error_state_scl.txt COPYONLY)

configure_file(resources/obs_failure_success_state_msg.txt resources/obs_failure_success_state_msg.txt COPYONLY)
configure_file(resources/obs_failure_success_state_spin.txt resources/obs_failure_success_state_spin.txt COPYONLY)
configure_file(resources/obs_failure_success_state_scl.txt resources/obs_failure_success_state_scl.txt COPYONLY)

configure_file(resources/stop_condition_empty_msg.txt resources/stop_condition_empty_msg.txt COPYONLY)
configure_file(resources/stop_condition_empty_spin.txt resources/stop_condition_empty_spin.txt COPYONLY)
configure_file(resources/stop_condition_empty_scl.txt resources/stop_condition_empty_scl.txt COPYONLY)

configure_file(resources/data_constraint_msg.txt resources/data_constraint_msg.txt COPYONLY)
configure_file(resources/data_constraint_spin.txt resources/data_constraint_spin.txt COPYONLY)
configure_file(resources/data_constraint_scl.txt resources/data_constraint_scl.txt COPYONLY)

configure_file(resources/data_constraint_nested_msg.txt resources/data_constraint_nested_msg.txt COPYONLY)
configure_file(resources/data_constraint_nested_spin.txt resources/data_constraint_nested_spin.txt COPYONLY)
configure_file(resources/data_constraint_nested_scl.txt resources/data_constraint_nested_scl.txt COPYONLY)

configure_file(resources/scenario.sim resources/scenario.sim COPYONLY)

                
add_format_target(${TEST_NAME})
