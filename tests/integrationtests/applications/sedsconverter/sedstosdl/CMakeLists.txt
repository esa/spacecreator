set(TEST_NAME tst_sedsconverter_sedstosdl)
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

configure_file(test_state_machine_with_bare_inputs.sh test_state_machine_with_bare_inputs.sh COPYONLY)
configure_file(resources/state_machine_with_bare_inputs.xml resources/state_machine_with_bare_inputs.xml COPYONLY)
configure_file(resources/state_machine_with_bare_inputs.output resources/state_machine_with_bare_inputs.output COPYONLY)

configure_file(test_led_demo.sh test_led_demo.sh COPYONLY)
configure_file(resources/test_led_demo.xml resources/test_led_demo.xml COPYONLY)
configure_file(resources/test_led_demo.output resources/test_led_demo.output COPYONLY)
configure_file(resources/test_led_demo.system_structure resources/test_led_demo.system_structure COPYONLY)

configure_file(test_calculator.sh test_calculator.sh COPYONLY)
configure_file(resources/test_calculator.xml resources/test_calculator.xml COPYONLY)
configure_file(resources/test_calculator.output resources/test_calculator.output COPYONLY)
configure_file(resources/test_calculator.system_structure resources/test_calculator.system_structure COPYONLY)

configure_file(test_comparator.sh test_comparator.sh COPYONLY)
configure_file(resources/test_comparator.xml resources/test_comparator.xml COPYONLY)
configure_file(resources/test_comparator.output resources/test_comparator.output COPYONLY)
configure_file(resources/test_comparator.system_structure resources/test_comparator.system_structure COPYONLY)

configure_file(test_factorial.sh test_factorial.sh COPYONLY)
configure_file(resources/test_factorial.xml resources/test_factorial.xml COPYONLY)
configure_file(resources/test_factorial.output resources/test_factorial.output COPYONLY)
configure_file(resources/test_factorial.system_structure resources/test_factorial.system_structure COPYONLY)
