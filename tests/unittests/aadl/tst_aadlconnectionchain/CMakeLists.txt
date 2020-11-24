set(TEST_NAME tst_aadlconnectionchain)
project(${TEST_NAME})

add_executable(${TEST_NAME}
    connectionchains.xml
    tst_aadlconnectionchain.cpp
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME} PUBLIC
    aadl
    Qt5::Test
)
