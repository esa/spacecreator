set(TEST_NAME tst_simulinkxmlimporter)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(${TEST_NAME}
    PRIVATE     Conversion::Common
                SIMULINK::XmlImporter
                Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test
                Qt${QT_VERSION_MAJOR}::Xml
                CommonProjectWarnings)

configure_file(resources/Complex.xml Complex.xml COPYONLY)

add_format_target(${TEST_NAME})
