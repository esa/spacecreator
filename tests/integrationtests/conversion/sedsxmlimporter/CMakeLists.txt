set(TEST_NAME tsti_sedsxmlimporter)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(${TEST_NAME}
    PRIVATE     Conversion::Common
                SEDS::Model
                SEDS::XmlImporter
                Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test
                CommonProjectWarnings)

configure_file(resources/external_references.toml external_references.toml COPYONLY)
configure_file(resources/seds.xml seds.xml COPYONLY)
configure_file(resources/some_data.xml some_data.xml COPYONLY)
configure_file(resources/seds-core-semantics.xsd seds-core-semantics.xsd COPYONLY)
configure_file(resources/seds-extension-semantics.xsd seds-extension-semantics.xsd COPYONLY)
configure_file(resources/seds.xsd seds.xsd COPYONLY)

add_format_target(${TEST_NAME})
