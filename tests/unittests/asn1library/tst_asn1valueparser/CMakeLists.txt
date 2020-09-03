set(TEST_NAME tst_asn1valueparser)
project(${TEST_NAME})

add_executable(${TEST_NAME}
    tst_asn1valueparser.cpp
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME} PUBLIC
    asn1editor
    Qt5::Core
    Qt5::Test
)
