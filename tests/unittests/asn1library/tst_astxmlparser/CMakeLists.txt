set(TEST_NAME tst_astxmlparser)
project(${TEST_NAME})

add_executable(${TEST_NAME}
    astxmlparser_tests.cpp
    astxmlparser_tests.h
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME} PUBLIC
    asn1library
    Qt5::Core
    Qt5::Test
)
