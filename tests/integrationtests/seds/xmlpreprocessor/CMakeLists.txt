set(TEST_NAME tsti_xmlpreprocessor)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_link_libraries(${TEST_NAME}
    PRIVATE     SEDS::TestBase
                SEDS::XmlPreprocessor
                Qt5::Xml)

configure_file(resources/Valid.xml Valid.xml COPYONLY)
configure_file(resources/IncludesValid.xml IncludesValid.xml COPYONLY)
configure_file(resources/UndefinedExternalReference.xml UndefinedExternalReference.xml COPYONLY)

# add_format_target(${TEST_NAME})
