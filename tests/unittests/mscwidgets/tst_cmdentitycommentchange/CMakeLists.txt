set(TEST_NAME tst_cmdentitycommentchange)

add_executable(${TEST_NAME}
    tst_cmdentitycommentchange.cpp
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME} PUBLIC
    mscwidgets
    Qt5::Core
    Qt5::Test
)
