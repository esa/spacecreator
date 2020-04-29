set(TEST_NAME tst_commandlineparser)

add_executable(${TEST_NAME}
    tst_commandlineparser.cpp
    ../../../../msceditor/commandlineparser.cpp
    ../../../../msceditor/commandlineparser.h
    ../../../../msceditor/mainwindow.cpp
    ../../../../msceditor/mainwindow.h
    ../../../../msceditor/mainwindow.ui
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME} -xunitxml -o ${CMAKE_BINARY_DIR}/${TEST_NAME}_result.xml)

target_link_libraries(${TEST_NAME} PUBLIC
    application
    Qt5::Core
    Qt5::Test
    Qt5::Widgets
    Qt5::Xml
)

target_include_directories(${TEST_NAME} PRIVATE
    ../../../../msceditor
)

target_compile_definitions(${TEST_NAME} PUBLIC EXAMPLES_STORAGE_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/../../../../examples/\")
