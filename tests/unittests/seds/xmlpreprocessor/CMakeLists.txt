set(TEST_NAME tst_xmlpreprocessor)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(${TEST_NAME}
    PRIVATE     Conversion::Common
                SEDS::XmlPreprocessor
                Qt5::Core
                Qt5::Test
                Qt5::Xml)

configure_file(resources/Valid.xml Valid.xml COPYONLY)
configure_file(resources/IncludesValid.xml IncludesValid.xml COPYONLY)
configure_file(resources/UndefinedExternalReference.xml UndefinedExternalReference.xml COPYONLY)

add_format_target(${TEST_NAME})
