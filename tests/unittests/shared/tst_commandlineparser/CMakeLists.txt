set(TEST_NAME tst_commandlineparser)

add_executable(${TEST_NAME}
    tst_commandlineparser.cpp
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME} PUBLIC
    shared
    libmsceditor
    Qt5::Core
    Qt5::Test
    Qt5::Widgets
    Qt5::Xml
)

target_include_directories(${TEST_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src/libmsceditors/msceditor/msceditor
)

target_compile_definitions(${TEST_NAME} PUBLIC EXAMPLES_STORAGE_PATH=\"${CMAKE_SOURCE_DIR}/examples/msc/\")
