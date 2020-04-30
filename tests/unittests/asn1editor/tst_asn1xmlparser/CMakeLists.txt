set(TEST_NAME tst_asn1xmlparser)

add_executable(${TEST_NAME}
    tst_asn1xmlparser.cpp
    boolenum_type.xml
    choice_type.xml
    empty.xml
    invalid_format.xml
    number_type.xml
    sequence_type.xml
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME} -xunitxml -o ${CMAKE_BINARY_DIR}/${TEST_NAME}_result.xml)

target_link_libraries(${TEST_NAME} PUBLIC
    asn1editor
    Qt5::Core
    Qt5::Test
)
