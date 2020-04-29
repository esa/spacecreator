set(TEST_NAME tst_asn1itemmodel)

add_executable(${TEST_NAME}
    tst_asn1itemmodel.cpp
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME} -xunitxml -o ${CMAKE_BINARY_DIR}/${TEST_NAME}_result.xml)

target_link_libraries(${TEST_NAME} PUBLIC
    asn1editor
    Qt5::Core
    Qt5::Test
)
