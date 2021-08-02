set(TEST_NAME tsti_sedsxmlparser)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_link_libraries(${TEST_NAME}
    PRIVATE     SEDS::TestBase
                SEDS::Parser
                Qt5::Xml)

configure_file(resources/Arrays.xml Arrays.xml COPYONLY)
configure_file(resources/Components.xml Components.xml COPYONLY)
configure_file(resources/Containers.xml Containers.xml COPYONLY)
configure_file(resources/Device.xml Device.xml COPYONLY)
configure_file(resources/Interfaces.xml Interfaces.xml COPYONLY)
configure_file(resources/SimpleDataTypes.xml SimpleDataTypes.xml COPYONLY)

# add_format_target(${TEST_NAME})
