set(TEST_NAME tst_attributesconfigure)
project(${TEST_NAME})

add_executable(${TEST_NAME}
    tst_attributesconfigure.cpp
    ${IV_SOURCES}
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME} PUBLIC
    aadl
    libiveditor
    Qt5::Core
    Qt5::Xml
    Qt5::Test
)

target_include_directories(${TEST_NAME} PRIVATE
    ${IV_DIR}
)
