set(TEST_NAME tsti_100messages)
project(${TEST_NAME})

add_executable(${TEST_NAME}
    tsti_100messages.cpp
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME} PUBLIC
    commontestlib
    libmsceditor
    ${QT_CORE}
    ${QT_TEST}
)
