set(TEST_NAME tst_messagecreatortool)
project(${TEST_NAME})

add_executable(${TEST_NAME}
    tst_messagecreatortool.cpp
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME} PUBLIC
    commontestlib
    libmsceditor
    Qt5::Core
    Qt5::Test
)
