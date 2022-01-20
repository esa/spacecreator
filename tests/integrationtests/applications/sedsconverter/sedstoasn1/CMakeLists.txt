set(TEST_NAME tst_sedsconverter_sedstoasn1)
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

configure_file(test_simple_data_types.sh test_simple_data_types.sh COPYONLY)
configure_file(resources/test_simple_data_types.xml resources/test_simple_data_types.xml COPYONLY)
configure_file(resources/test_simple_data_types_asn.output resources/test_simple_data_types_asn.output COPYONLY)
configure_file(resources/test_simple_data_types_acn.output resources/test_simple_data_types_acn.output COPYONLY)

configure_file(test_arrays.sh test_arrays.sh COPYONLY)
configure_file(resources/test_arrays.xml resources/test_arrays.xml COPYONLY)
configure_file(resources/test_arrays_asn.output resources/test_arrays_asn.output COPYONLY)
configure_file(resources/test_arrays_acn.output resources/test_arrays_acn.output COPYONLY)

configure_file(test_indexed_array.sh test_indexed_array.sh COPYONLY)
configure_file(resources/test_indexed_array.xml resources/test_indexed_array.xml COPYONLY)
configure_file(resources/test_indexed_array_asn.output resources/test_indexed_array_asn.output COPYONLY)
configure_file(resources/test_indexed_array_acn.output resources/test_indexed_array_acn.output COPYONLY)

configure_file(test_containers.sh test_containers.sh COPYONLY)
configure_file(resources/test_containers.xml resources/test_containers.xml COPYONLY)
configure_file(resources/test_containers_asn.output resources/test_containers_asn.output COPYONLY)
configure_file(resources/test_containers_acn.output resources/test_containers_acn.output COPYONLY)

configure_file(test_container_constraints.sh test_container_constraints.sh COPYONLY)
configure_file(resources/test_container_constraints.xml resources/test_container_constraints.xml COPYONLY)
configure_file(resources/test_container_constraints_asn.output resources/test_container_constraints_asn.output COPYONLY)

configure_file(test_container_entries.sh test_container_entries.sh COPYONLY)
configure_file(resources/test_container_entries.xml resources/test_container_entries.xml COPYONLY)
configure_file(resources/test_container_entries_asn.output resources/test_container_entries_asn.output COPYONLY)
configure_file(resources/test_container_entries_acn.output resources/test_container_entries_acn.output COPYONLY)

configure_file(test_container_inheritance.sh test_container_inheritance.sh COPYONLY)
configure_file(resources/test_container_inheritance.xml resources/test_container_inheritance.xml COPYONLY)
configure_file(resources/test_container_inheritance_asn.output resources/test_container_inheritance_asn.output COPYONLY)
configure_file(resources/test_container_inheritance_acn.output resources/test_container_inheritance_acn.output COPYONLY)

configure_file(test_min_max_range.sh test_min_max_range.sh COPYONLY)
configure_file(resources/test_min_max_range.xml resources/test_min_max_range.xml COPYONLY)
configure_file(resources/test_min_max_range_asn.output resources/test_min_max_range_asn.output COPYONLY)
