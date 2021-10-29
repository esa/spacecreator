set(TEST_NAME tsti_asn1importer)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(${TEST_NAME}
    PRIVATE     Conversion::Common
                ASN1::Importer
                Qt5::Core
                Qt5::Test
                CommonProjectWarnings)

configure_file(resources/Valid.asn Valid.asn COPYONLY)
configure_file(resources/Invalid.asn Invalid.asn COPYONLY)

add_format_target(${TEST_NAME})
