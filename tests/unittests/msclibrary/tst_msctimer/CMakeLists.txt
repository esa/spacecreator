set(TEST_NAME tst_msctimer)
project(${TEST_NAME})

add_executable(${TEST_NAME}
    tst_msctimer.cpp
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME} PUBLIC
    msclibrary
    Qt5::Core
    Qt5::Test
)
