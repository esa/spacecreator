set(TEST_NAME tst_sedsconverter_sedstoiv)
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

add_dependencies(${TEST_NAME} SedsConverter)

add_format_target(${TEST_NAME})

configure_file(resources/config.xml resources/config.xml COPYONLY)

configure_file(resources/xml2dv/function_properties.tmplt resources/xml2dv/function_properties.tmplt COPYONLY)
configure_file(resources/xml2dv/function_subcomponents.tmplt resources/xml2dv/function_subcomponents.tmplt COPYONLY)
configure_file(resources/xml2dv/function_with.tmplt resources/xml2dv/function_with.tmplt COPYONLY)
configure_file(resources/xml2dv/interfaceview.tmplt resources/xml2dv/interfaceview.tmplt COPYONLY)

configure_file(resources/xml2iv/comment.tmplt resources/xml2iv/comment.tmplt COPYONLY)
configure_file(resources/xml2iv/comments.tmplt resources/xml2iv/comments.tmplt COPYONLY)
configure_file(resources/xml2iv/connection.tmplt resources/xml2iv/connection.tmplt COPYONLY)
configure_file(resources/xml2iv/connectiongroup.tmplt resources/xml2iv/connectiongroup.tmplt COPYONLY)
configure_file(resources/xml2iv/connections.tmplt resources/xml2iv/connections.tmplt COPYONLY)
configure_file(resources/xml2iv/function.tmplt resources/xml2iv/function.tmplt COPYONLY)
configure_file(resources/xml2iv/functions.tmplt resources/xml2iv/functions.tmplt COPYONLY)
configure_file(resources/xml2iv/interface.tmplt resources/xml2iv/interface.tmplt COPYONLY)
configure_file(resources/xml2iv/interfaces.tmplt resources/xml2iv/interfaces.tmplt COPYONLY)
configure_file(resources/xml2iv/interfaceview.tmplt resources/xml2iv/interfaceview.tmplt COPYONLY)


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

configure_file(test_command_arguments.sh test_command_arguments.sh COPYONLY)
configure_file(resources/test_command_arguments.xml resources/test_command_arguments.xml COPYONLY)
configure_file(resources/test_command_arguments.output resources/test_command_arguments.output COPYONLY)
configure_file(resources/test_command_arguments.asn resources/test_command_arguments.asn COPYONLY)

configure_file(test_parameters.sh test_parameters.sh COPYONLY)
configure_file(resources/test_parameters.xml resources/test_parameters.xml COPYONLY)
configure_file(resources/test_parameters.output resources/test_parameters.output COPYONLY)
