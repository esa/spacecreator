set(TEST_NAME tst_entitydeletetool)
project(${TEST_NAME})

add_executable(${TEST_NAME}
    tst_entitydeletetool.cpp
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME} PUBLIC
    libmsceditor
    Qt5::Core
    Qt5::Test
)
