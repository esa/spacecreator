
set(TEST_NAME tst_testdriver)
project(${TEST_NAME})

add_executable(${TEST_NAME})

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     C/src/testdriver.c
                C/asn1crt_encoding_acn.c
                C/asn1crt_encoding_uper.c
                C/asn1crt_encoding.c
                C/asn1crt.c
                C/dataview-uniq.c
                C/debug_messages.c
                C/PrintTypes.c
                C/timeInMS.c
                main.c
)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/C/src
                ${CMAKE_SOURCE_DIR}/C
)
