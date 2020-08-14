set(TEST_NAME tst_typestreemodel)
project(${TEST_NAME})

add_executable(${TEST_NAME}
    typestreemodel_tests.cpp
    typestreemodel_tests.h
    ../common/modeltest.h
    ../common/modeltest.cpp
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

set(QTC_LIBRARIES
    -L"${QTC_LIB_DIR}"
    -L"${QTC_PLUGINS_DIR}"
    Aggregation
    ExtensionSystem
    QtcSsh
    Utils
)
set(QTC_PLUGINS
    Core
    ProjectExplorer
    TextEditor
)

target_link_libraries(${TEST_NAME} PUBLIC
    Asn1Acn
    asn1library
    ${QTC_LIBRARIES}
    ${QTC_PLUGINS}
    Qt5::Core
    Qt5::Gui
    Qt5::Test
    Qt5::Widgets
)

target_include_directories(${TEST_NAME}
    PUBLIC ../common
)

set_tests_properties(${TEST_NAME} PROPERTIES
    ENVIRONMENT "LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${QTC_LIB_DIR}:${QTC_PLUGINS_DIR}"
)
