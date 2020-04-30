set(TEST_NAME tst_commandlineparser)

add_executable(${TEST_NAME}
    tst_commandlineparser.cpp
    ${CMAKE_SOURCE_DIR}/src/applications/msceditor/msceditor/commandlineparser.cpp
    ${CMAKE_SOURCE_DIR}/src/applications/msceditor/msceditor/commandlineparser.h
    ${CMAKE_SOURCE_DIR}/src/applications/msceditor/msceditor/mainwindow.cpp
    ${CMAKE_SOURCE_DIR}/src/applications/msceditor/msceditor/mainwindow.h
    ${CMAKE_SOURCE_DIR}/src/applications/msceditor/msceditor/mainwindow.ui
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME} PUBLIC
    application
    Qt5::Core
    Qt5::Test
    Qt5::Widgets
    Qt5::Xml
)

target_include_directories(${TEST_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src/applications/msceditor/msceditor
)

target_compile_definitions(${TEST_NAME} PUBLIC EXAMPLES_STORAGE_PATH=\"${CMAKE_SOURCE_DIR}/examples/msc/\")
