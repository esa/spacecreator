set(TEST_NAME tst_aadlobjectsmodel)
project(${TEST_NAME})

add_executable(${TEST_NAME}
    tst_aadlobjectsmodel.cpp
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME} PUBLIC
    aadl
    Qt5::Test
)
