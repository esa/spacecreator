set(TEST_NAME tst_aadlobjectfunctiontype)
project(${TEST_NAME})

add_executable(${TEST_NAME}
    tst_aadlobjectfunctiontype.cpp
    ${IV_SOURCES}
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME} PUBLIC
    aadl
    Qt5::Test
)

target_include_directories(${TEST_NAME} PRIVATE
    ${IV_DIR}
    ..
)
