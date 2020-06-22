set(TEST_NAME tst_itemzorder)
project(${TEST_NAME})

add_executable(${TEST_NAME}
    tst_itemzorder.cpp
    ${IV_SOURCES}
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME} PUBLIC
    aadl
    libiveditor
    Qt5::Core
    Qt5::Test
    Qt5::Gui
    Qt5::Widgets
    Qt5::Test
)

target_include_directories(${TEST_NAME} PRIVATE
    ${IV_DIR}
)
