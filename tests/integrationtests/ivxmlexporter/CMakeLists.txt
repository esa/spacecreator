set(TEST_NAME tsti_ivxmlexporter)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(${TEST_NAME}
    PRIVATE     Conversion::Common
                Conversion::IvXmlExporter
                ivcore
                Qt5::Core
                Qt5::Test)

configure_file(resources/config.xml config.xml COPYONLY)

add_format_target(${TEST_NAME})
