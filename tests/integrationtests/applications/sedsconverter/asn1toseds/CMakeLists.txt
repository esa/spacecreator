set(TEST_NAME tst_sedsconverter_asn1toseds)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_testXML(${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/tests)
target_link_libraries(${TEST_NAME}
    PRIVATE     Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test)

add_format_target(${TEST_NAME})

configure_file(test_simple_data_types.sh test_simple_data_types.sh COPYONLY)
configure_file(resources/test_simple_data_types.output resources/test_simple_data_types.output COPYONLY)
configure_file(resources/test_simple_data_types.asn resources/test_simple_data_types.asn COPYONLY)
configure_file(resources/test_simple_data_types.acn resources/test_simple_data_types.acn COPYONLY)

configure_file(test_sequences.sh test_sequences.sh COPYONLY)
configure_file(resources/test_sequences.output resources/test_sequences.output COPYONLY)
configure_file(resources/test_sequences.asn resources/test_sequences.asn COPYONLY)
configure_file(resources/test_sequences.acn resources/test_sequences.acn COPYONLY)

configure_file(test_sequences_of.sh test_sequences_of.sh COPYONLY)
configure_file(resources/test_sequences_of.output resources/test_sequences_of.output COPYONLY)
configure_file(resources/test_sequences_of.asn resources/test_sequences_of.asn COPYONLY)
configure_file(resources/test_sequences_of.acn resources/test_sequences_of.acn COPYONLY)

configure_file(test_choices.sh test_choices.sh COPYONLY)
configure_file(resources/test_choices.output resources/test_choices.output COPYONLY)
configure_file(resources/test_choices.asn resources/test_choices.asn COPYONLY)
configure_file(resources/test_choices.acn resources/test_choices.acn COPYONLY)

configure_file(test_with_components.sh test_with_components.sh COPYONLY)
configure_file(resources/test_with_components.output resources/test_with_components.output COPYONLY)
configure_file(resources/test_with_components.asn resources/test_with_components.asn COPYONLY)
configure_file(resources/test_with_components.acn resources/test_with_components.acn COPYONLY)

configure_file(test_multiple_asn_models.sh test_multiple_asn_models.sh COPYONLY)
configure_file(resources/test_multiple_models.output resources/test_multiple_models.output COPYONLY)
configure_file(resources/test_multiple_models.asn resources/test_multiple_models.asn COPYONLY)
configure_file(resources/test_multiple_models.acn resources/test_multiple_models.acn COPYONLY)

configure_file(test_skip_empty_sequences.sh test_skip_empty_sequences.sh COPYONLY)
configure_file(resources/test_skip_empty_sequences.output resources/test_skip_empty_sequences.output COPYONLY)
configure_file(resources/test_skip_empty_sequences.asn resources/test_skip_empty_sequences.asn COPYONLY)
configure_file(resources/test_skip_empty_sequences.acn resources/test_skip_empty_sequences.acn COPYONLY)
