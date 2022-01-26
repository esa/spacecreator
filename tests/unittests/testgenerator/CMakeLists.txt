set(TEST_NAME tst_testgenerator)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp
)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs
                ${CMAKE_SOURCE_DIR}/tests/unittests
)
target_link_libraries(${TEST_NAME}
    PRIVATE     Conversion::Common
                ivcore
                IV::XmlImporter
                Asn1Importer
                TestGenerator
                CsvImporter
                Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test
                CommonProjectWarnings
)

configure_file(resources/config.xml resources/config.xml COPYONLY)

configure_file(resources/test_data.csv resources/test_data.csv COPYONLY)

configure_file(resources/empty.csv resources/empty.csv COPYONLY)

configure_file(resources/nominal-interfaceview.xml resources/nominal-interfaceview.xml COPYONLY)
configure_file(resources/cyclicif-interfaceview.xml resources/cyclicif-interfaceview.xml COPYONLY)
configure_file(resources/implementation-interfaceview.xml resources/implementation-interfaceview.xml COPYONLY)

configure_file(resources/testgenerator.asn resources/testgenerator.asn COPYONLY)

configure_file(resources/testdriver.c.out resources/testdriver.c.out)

add_format_target(${TEST_NAME})
