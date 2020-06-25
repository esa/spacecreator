set(TEST_NAME tst_nameitem)
project(${TEST_NAME})

add_executable(${TEST_NAME}
    tst_nameitem.cpp
    ../tst_textitem/textitembasetest.cpp
    ../tst_textitem/textitembasetest.h
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME} PUBLIC
    commontestlib
    libmsceditor
    Qt5::Core
    Qt5::Test
)
