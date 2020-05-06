set(TEST_NAME tst_grippointshandler)

add_executable(${TEST_NAME}
    tst_grippointshandler.cpp
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME} PUBLIC
    shared
    Qt5::Core
    Qt5::Test
)
