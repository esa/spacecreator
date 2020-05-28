set(TEST_NAME tst_mscreader)
project(${TEST_NAME})

add_executable(${TEST_NAME}
    tst_mscreader.cpp
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME} PUBLIC
    msclibrary
    Qt5::Core
    Qt5::Test
)

target_compile_definitions(${TEST_NAME} PUBLIC EXAMPLES_DIR=\"${CMAKE_SOURCE_DIR}/examples/msc/\")
