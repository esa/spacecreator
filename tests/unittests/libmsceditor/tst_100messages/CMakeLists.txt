set(TEST_NAME tst_100messages)
project(${TEST_NAME})

add_executable(${TEST_NAME}
    tst_100messages.cpp
    ../common/tst_common.cpp
    ../common/tst_common.h
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME} PUBLIC
    libmsceditor
    Qt5::Core
    Qt5::Test
)

target_include_directories(${TEST_NAME} PUBLIC ../common)
