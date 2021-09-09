set(TEST_NAME tst_xmlvalidator)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(${TEST_NAME}
    PRIVATE     Conversion::Common
                SEDS::XmlValidator
                Qt5::Core
                Qt5::Test
                CommonProjectWarnings)

configure_file(resources/IncludesInvalid.xml IncludesInvalid.xml COPYONLY)
configure_file(resources/Invalid.xml Invalid.xml COPYONLY)
configure_file(resources/seds-core-semantics.xsd seds-core-semantics.xsd COPYONLY)
configure_file(resources/seds-extension-semantics.xsd seds-extension-semantics.xsd COPYONLY)
configure_file(resources/seds.xsd seds.xsd COPYONLY)
configure_file(resources/SomeInvalidData.xml SomeInvalidData.xml COPYONLY)
configure_file(resources/Valid.xml Valid.xml COPYONLY)

add_format_target(${TEST_NAME})
