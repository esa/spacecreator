set(TEST_NAME tst_grippoint)

add_executable(${TEST_NAME}
    tst_grippoint.cpp
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME} PUBLIC
    shared
    Qt5::Core
    Qt5::Test
)
