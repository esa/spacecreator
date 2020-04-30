set(TEST_NAME tst_documentitemmodel)

add_executable(${TEST_NAME}
    tst_documentitemmodel.cpp
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME} -xunitxml -o ${CMAKE_BINARY_DIR}/${TEST_NAME}_result.xml)

target_link_libraries(${TEST_NAME} PUBLIC
    application
    Qt5::Core
    Qt5::Test
)
