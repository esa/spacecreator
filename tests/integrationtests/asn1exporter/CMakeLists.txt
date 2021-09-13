set(TEST_NAME tsti_asn1exporter)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(${TEST_NAME}
    PRIVATE     Conversion::Common
                Conversion::Asn1Exporter
                Qt5::Core
                Qt5::Test)

configure_file(resources/Acn_file1.acn expect_Acn_file1.acn COPYONLY)
configure_file(resources/Acn_file2.acn expect_Acn_file2.acn COPYONLY)
configure_file(resources/Asn1_file1.asn expect_Asn1_file1.asn COPYONLY)
configure_file(resources/Asn1_file2.asn expect_Asn1_file2.asn COPYONLY)

add_format_target(${TEST_NAME})
