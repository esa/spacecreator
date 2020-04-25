set(TEST_NAME tst_remotecontrol)

add_executable(${TEST_NAME}
    tst_remotecontrol.cpp
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME} PUBLIC
    application
    Qt5::Core
    Qt5::Test
)
