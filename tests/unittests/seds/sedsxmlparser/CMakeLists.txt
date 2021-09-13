set(TEST_NAME tst_sedsxmlparser)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(${TEST_NAME}
    PRIVATE     Conversion::Common
                SEDS::XmlParser
                Qt5::Core
                Qt5::Test
                Qt5::Xml
                CommonProjectWarnings)

configure_file(resources/Arrays.xml Arrays.xml COPYONLY)
configure_file(resources/Components.xml Components.xml COPYONLY)
configure_file(resources/Containers.xml Containers.xml COPYONLY)
configure_file(resources/Device.xml Device.xml COPYONLY)
configure_file(resources/Interfaces.xml Interfaces.xml COPYONLY)
configure_file(resources/SimpleDataTypes.xml SimpleDataTypes.xml COPYONLY)

add_format_target(${TEST_NAME})
