set(TEST_NAME tst_sedsconverter_ivtoseds)
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

configure_file(resources/config.xml resources/config.xml COPYONLY)
configure_file(resources/seds.xsd resources/seds.xsd COPYONLY)
configure_file(resources/seds-core-semantics.xsd resources/seds-core-semantics.xsd COPYONLY)
configure_file(resources/seds-extension-semantics.xsd resources/seds-extension-semantics.xsd COPYONLY)
configure_file(resources/xml.xsd resources/xml.xsd COPYONLY)

configure_file(test_cyclic_interface.sh test_cyclic_interface.sh COPYONLY)
configure_file(resources/test_cyclic_interface.output resources/test_cyclic_interface.output COPYONLY)
configure_file(resources/test_cyclic_interface.xml resources/test_cyclic_interface.xml COPYONLY)

configure_file(test_protected_interface.sh test_protected_interface.sh COPYONLY)
configure_file(resources/test_protected_interface.output resources/test_protected_interface.output COPYONLY)
configure_file(resources/test_protected_interface.xml resources/test_protected_interface.xml COPYONLY)

configure_file(test_sporadic_interface.sh test_sporadic_interface.sh COPYONLY)
configure_file(resources/test_sporadic_interface.output resources/test_sporadic_interface.output COPYONLY)
configure_file(resources/test_sporadic_interface.xml resources/test_sporadic_interface.xml COPYONLY)

configure_file(test_unprotected_interface.sh test_unprotected_interface.sh COPYONLY)
configure_file(resources/test_unprotected_interface.output resources/test_unprotected_interface.output COPYONLY)
configure_file(resources/test_unprotected_interface.xml resources/test_unprotected_interface.xml COPYONLY)

configure_file(test_repeated_interface_name.sh test_repeated_interface_name.sh COPYONLY)
configure_file(resources/test_repeated_interface_name.output resources/test_repeated_interface_name.output COPYONLY)
configure_file(resources/test_repeated_interface_name.xml resources/test_repeated_interface_name.xml COPYONLY)

configure_file(test_pinger.sh test_pinger.sh COPYONLY)
configure_file(resources/test_pinger.output resources/test_pinger.output COPYONLY)
configure_file(resources/test_pinger.xml resources/test_pinger.xml COPYONLY)

configure_file(test_with_data_types.sh test_with_data_types.sh COPYONLY)
configure_file(resources/test_with_data_types.xml resources/test_with_data_types.xml COPYONLY)
configure_file(resources/test_with_data_types.output resources/test_with_data_types.output COPYONLY)
configure_file(resources/test_with_data_types.asn resources/test_with_data_types.asn COPYONLY)
configure_file(resources/test_with_data_types.acn resources/test_with_data_types.acn COPYONLY)

configure_file(test_function_to_convert.sh test_function_to_convert.sh COPYONLY)
configure_file(resources/test_function_to_convert.output resources/test_function_to_convert.output COPYONLY)
configure_file(resources/test_function_to_convert_interfaceview.xml resources/test_function_to_convert_interfaceview.xml COPYONLY)
