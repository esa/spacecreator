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
                Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test
                Qt${QT_VERSION_MAJOR}::Xml
                CommonProjectWarnings)

configure_file(resources/testExportsInteger.xml resources/testExportsInteger.xml COPYONLY)
configure_file(resources/testExportsEnum.xml resources/testExportsEnum.xml COPYONLY)
configure_file(resources/testExportsFloat.xml resources/testExportsFloat.xml COPYONLY)
configure_file(resources/testExportsString.xml resources/testExportsString.xml COPYONLY)
configure_file(resources/testExportsIntegerQt6.xml resources/testExportsIntegerQt6.xml COPYONLY)
configure_file(resources/testExportsEnumQt6.xml resources/testExportsEnumQt6.xml COPYONLY)
configure_file(resources/testExportsFloatQt6.xml resources/testExportsFloatQt6.xml COPYONLY)
configure_file(resources/testExportsStringQt6.xml resources/testExportsStringQt6.xml COPYONLY)

add_format_target(${TEST_NAME})
