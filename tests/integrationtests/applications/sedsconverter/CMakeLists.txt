set(TEST_NAME tst_sedsconverter)
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

configure_file(resources/Makefile resources/Makefile COPYONLY)

configure_file(test_simple_data_types.sh test_simple_data_types.sh COPYONLY)
configure_file(resources/test_simple_data_types.xml resources/test_simple_data_types.xml COPYONLY)
configure_file(resources/test_simple_data_types_asn.output resources/test_simple_data_types_asn.output COPYONLY)
configure_file(resources/test_simple_data_types_acn.output resources/test_simple_data_types_acn.output COPYONLY)

configure_file(test_arrays.sh test_arrays.sh COPYONLY)
configure_file(resources/test_arrays.xml resources/test_arrays.xml COPYONLY)
configure_file(resources/test_arrays_asn.output resources/test_arrays_asn.output COPYONLY)
configure_file(resources/test_arrays_acn.output resources/test_arrays_acn.output COPYONLY)

configure_file(test_containers.sh test_containers.sh COPYONLY)
configure_file(resources/test_containers.xml resources/test_containers.xml COPYONLY)
configure_file(resources/test_containers_asn.output resources/test_containers_asn.output COPYONLY)
configure_file(resources/test_containers_acn.output resources/test_containers_acn.output COPYONLY)

configure_file(test_datatype_inside_component.sh test_datatype_inside_component.sh COPYONLY)
configure_file(resources/test_datatype_inside_component.xml resources/test_datatype_inside_component.xml COPYONLY)
configure_file(resources/test_datatype_inside_component_asn1.output resources/test_datatype_inside_component_asn1.output COPYONLY)
configure_file(resources/test_datatype_inside_component_acn1.output resources/test_datatype_inside_component_acn1.output COPYONLY)
configure_file(resources/test_datatype_inside_component_asn2.output resources/test_datatype_inside_component_asn2.output COPYONLY)
configure_file(resources/test_datatype_inside_component_acn2.output resources/test_datatype_inside_component_acn2.output COPYONLY)

configure_file(test_required_interfaces.sh test_required_interfaces.sh COPYONLY)
configure_file(resources/test_required_interfaces.xml resources/test_required_interfaces.xml COPYONLY)
configure_file(resources/test_required_interfaces.output resources/test_required_interfaces.output COPYONLY)

configure_file(test_provided_interfaces.sh test_provided_interfaces.sh COPYONLY)
configure_file(resources/test_provided_interfaces.xml resources/test_provided_interfaces.xml COPYONLY)
configure_file(resources/test_provided_interfaces.output resources/test_provided_interfaces.output COPYONLY)

configure_file(test_hwas.sh test_hwas.sh COPYONLY)
configure_file(resources/test_hwas.xml resources/test_hwas.xml COPYONLY)
configure_file(resources/test_hwas.interfaceview resources/test_hwas.interfaceview COPYONLY)
configure_file(resources/test_hwas.hwasc resources/test_hwas.hwasc COPYONLY)

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
