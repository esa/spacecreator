set(TEST_NAME tst_aadlobjectsmodel)

add_executable(${TEST_NAME}
    tst_aadlobjectsmodel.cpp
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME} PUBLIC
    aadl
    Qt5::Concurrent
    Qt5::Network
    Qt5::Svg
    Qt5::Test
    Qt5::Widgets
    Grantlee5::Templates
)
