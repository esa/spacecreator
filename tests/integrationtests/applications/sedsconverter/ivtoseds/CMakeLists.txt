set(TEST_NAME tst_sedsconverter_ivtoseds)
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

configure_file(resources/config.xml resources/config.xml COPYONLY)
configure_file(resources/seds.xsd resources/seds.xsd COPYONLY)
configure_file(resources/seds-core-semantics.xsd resources/seds-core-semantics.xsd COPYONLY)
configure_file(resources/seds-extension-semantics.xsd resources/seds-extension-semantics.xsd COPYONLY)

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
