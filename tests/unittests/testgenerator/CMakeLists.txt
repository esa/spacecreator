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
                TestGenerator
                CsvImporter
                Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test
                CommonProjectWarnings
)

configure_file(resources/config.xml config.xml COPYONLY)
configure_file(resources/test_data.csv test_data.csv COPYONLY)
configure_file(resources/empty.csv empty.csv COPYONLY)
configure_file(resources/functionUnderTest1-interfaceview.xml interfaceview.xml COPYONLY)

add_format_target(${TEST_NAME})
