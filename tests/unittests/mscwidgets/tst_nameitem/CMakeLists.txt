set(TEST_NAME tst_nameitem)

add_executable(${TEST_NAME}
    tst_nameitem.cpp
    ../tst_textitem/textitembasetest.cpp
    ../tst_textitem/textitembasetest.h
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME} -xunitxml -o ${CMAKE_BINARY_DIR}/${TEST_NAME}_result.xml)

target_link_libraries(${TEST_NAME} PUBLIC
    mscwidgets
    Qt5::Core
    Qt5::Test
)
