set(TEST_NAME tsti_symboldefinitionreader)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_link_libraries(${TEST_NAME}
    PRIVATE     SEDS::TestBase
                SEDS::SymbolDefinitionReader)

configure_file(resources/Valid.toml Valid.toml COPYONLY)
configure_file(resources/Invalid.toml Invalid.toml COPYONLY)

add_format_target(${TEST_NAME})
