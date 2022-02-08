set(TEST_NAME tst_promelaexporter)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(${TEST_NAME}
    PRIVATE     Conversion::Common
                Promela::Exporter
                Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test)

configure_file(resources/expect_promela_mtype.pml expect_promela_mtype.pml COPYONLY)
configure_file(resources/expect_promela_named_mtype.pml expect_promela_named_mtype.pml COPYONLY)
configure_file(resources/expect_promela_utype.pml expect_promela_utype.pml COPYONLY)
configure_file(resources/expect_promela_utype_visibility.pml expect_promela_utype_visibility.pml COPYONLY)
configure_file(resources/expect_promela_utype_union.pml expect_promela_utype_union.pml COPYONLY)
configure_file(resources/expect_promela_type_alias.pml expect_promela_type_alias.pml COPYONLY)
configure_file(resources/expect_promela_value_definition.pml expect_promela_value_definition.pml COPYONLY)
configure_file(resources/expect_promela_declaration.pml expect_promela_declaration.pml COPYONLY)
configure_file(resources/expect_promela_includes.pml expect_promela_includes.pml COPYONLY)
configure_file(resources/expect_promela_channel_initialization.pml expect_promela_channel_initialization.pml COPYONLY)
configure_file(resources/expect_promela_basic_proctypes.pml expect_promela_basic_proctypes.pml COPYONLY)
configure_file(resources/expect_promela_init_proctype.pml expect_promela_init_proctype.pml COPYONLY)
configure_file(resources/expect_promela_proctype_elements.pml expect_promela_proctype_elements.pml COPYONLY)
configure_file(resources/expect_promela_do_loop.pml expect_promela_do_loop.pml COPYONLY)
configure_file(resources/expect_promela_epilogue_includes.pml expect_promela_epilogue_includes.pml COPYONLY)
configure_file(resources/expect_promela_conditional.pml expect_promela_conditional.pml COPYONLY)

add_format_target(${TEST_NAME})
