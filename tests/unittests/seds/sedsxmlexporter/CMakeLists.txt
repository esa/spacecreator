set(TEST_NAME tst_sedsxmlexporter)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs
                ${CMAKE_SOURCE_DIR}/tests
                ${CMAKE_SOURCE_DIR}/tests/unittests/conversion/common)
target_link_libraries(${TEST_NAME}
    PRIVATE     Conversion::Common
                ConversionTests::SedsModelBuilder
                SedsXmlExporter
                Qt5::Core
                Qt5::Test
                Qt5::Xml
                CommonProjectWarnings)

configure_file(resources/testExportsInteger.xml resources/testExportsInteger.xml COPYONLY)
configure_file(resources/testExportsEnum.xml resources/testExportsEnum.xml COPYONLY)

add_format_target(${TEST_NAME})
