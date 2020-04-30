set(TEST_NAME tst_100messages)

add_executable(${TEST_NAME}
    tst_100messages.cpp
    ../common/tst_common.cpp
    ../common/tst_common.h
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME} -xunitxml -o ${CMAKE_BINARY_DIR}/${TEST_NAME}_result.xml)

target_link_libraries(${TEST_NAME} PUBLIC
    mscwidgets
    Qt5::Core
    Qt5::Test
)

target_include_directories(${TEST_NAME} PUBLIC ../common)
