set(TEST_NAME tst_aadlnamevalidator)
project(${TEST_NAME})

add_executable(${TEST_NAME}
    tst_aadlnamevalidator.cpp
    ${IV_SOURCES}
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME} -xunitxml -o ${CMAKE_BINARY_DIR}/${TEST_NAME}.xml)

target_link_libraries(${TEST_NAME} PUBLIC
    aadl
    Qt5::Test
)

target_include_directories(${TEST_NAME} PRIVATE
    ${IV_DIR}
)
