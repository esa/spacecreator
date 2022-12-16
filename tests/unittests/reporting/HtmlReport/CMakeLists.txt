set(TEST_NAME tst_htmlreportbuilder)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     tst_htmlreportbuilder.cpp
                tst_tracebuilder.cpp
    PRIVATE     main.cpp
                tst_htmlreportbuilder.h
                tst_tracebuilder.h)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs
                ${CMAKE_SOURCE_DIR}/tests)
target_link_libraries(${TEST_NAME}
    PRIVATE     HtmlReport
                Report
                Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test)

configure_file(resources/msc_failure_mscdemo.msc resources/msc_failure_mscdemo.msc COPYONLY)
configure_file(resources/msc_failure_msg.txt resources/msc_failure_msg.txt COPYONLY)
configure_file(resources/msc_failure_spin.txt resources/msc_failure_spin.txt COPYONLY)
configure_file(resources/msc_failure_scl.txt resources/msc_failure_scl.txt COPYONLY)
configure_file(resources/msc_failure_result.html resources/msc_failure_result.html COPYONLY)
                
configure_file(resources/obs_failure_error_state_msg.txt resources/obs_failure_error_state_msg.txt COPYONLY)
configure_file(resources/obs_failure_error_state_spin.txt resources/obs_failure_error_state_spin.txt COPYONLY)
configure_file(resources/obs_failure_error_state_scl.txt resources/obs_failure_error_state_scl.txt COPYONLY)
configure_file(resources/obs_failure_error_state_result.html resources/obs_failure_error_state_result.html COPYONLY)

configure_file(resources/obs_failure_success_state_msg.txt resources/obs_failure_success_state_msg.txt COPYONLY)
configure_file(resources/obs_failure_success_state_spin.txt resources/obs_failure_success_state_spin.txt COPYONLY)
configure_file(resources/obs_failure_success_state_scl.txt resources/obs_failure_success_state_scl.txt COPYONLY)
configure_file(resources/obs_failure_success_state_result.html resources/obs_failure_success_state_result.html COPYONLY)

configure_file(resources/stop_condition_empty_msg.txt resources/stop_condition_empty_msg.txt COPYONLY)
configure_file(resources/stop_condition_empty_spin.txt resources/stop_condition_empty_spin.txt COPYONLY)
configure_file(resources/stop_condition_empty_scl.txt resources/stop_condition_empty_scl.txt COPYONLY)
configure_file(resources/stop_condition_empty_result.html resources/stop_condition_empty_result.html COPYONLY)

configure_file(resources/stop_condition_eventually_msg.txt resources/stop_condition_eventually_msg.txt COPYONLY)
configure_file(resources/stop_condition_eventually_spin.txt resources/stop_condition_eventually_spin.txt COPYONLY)
configure_file(resources/stop_condition_eventually_scl.txt resources/stop_condition_eventually_scl.txt COPYONLY)
configure_file(resources/stop_condition_eventually_result.html resources/stop_condition_eventually_result.html COPYONLY)

configure_file(resources/stop_condition_never_msg.txt resources/stop_condition_never_msg.txt COPYONLY)
configure_file(resources/stop_condition_never_spin.txt resources/stop_condition_never_spin.txt COPYONLY)
configure_file(resources/stop_condition_never_scl.txt resources/stop_condition_never_scl.txt COPYONLY)
configure_file(resources/stop_condition_never_result.html resources/stop_condition_never_result.html COPYONLY)

configure_file(resources/data_constraint_msg.txt resources/data_constraint_msg.txt COPYONLY)
configure_file(resources/data_constraint_spin.txt resources/data_constraint_spin.txt COPYONLY)
configure_file(resources/data_constraint_scl.txt resources/data_constraint_scl.txt COPYONLY)
configure_file(resources/data_constraint_result.html resources/data_constraint_result.html COPYONLY)

configure_file(resources/data_constraint_nested_msg.txt resources/data_constraint_nested_msg.txt COPYONLY)
configure_file(resources/data_constraint_nested_spin.txt resources/data_constraint_nested_spin.txt COPYONLY)
configure_file(resources/data_constraint_nested_scl.txt resources/data_constraint_nested_scl.txt COPYONLY)
configure_file(resources/data_constraint_nested_result.html resources/data_constraint_nested_result.html COPYONLY)

configure_file(resources/empty_result.html resources/empty_result.html COPYONLY)

configure_file(resources/scenario.sim resources/scenario.sim COPYONLY)
configure_file(resources/template.html resources/template.html COPYONLY)

add_format_target(${TEST_NAME})
