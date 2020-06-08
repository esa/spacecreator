set(TEST_NAME tst_projectcontenthandler)
project(${TEST_NAME})

add_executable(${TEST_NAME}
    projectcontenthandler_tests.cpp
    projectcontenthandler_tests.h
    documentprocessorstub.cpp
    documentprocessorstub.h
    sourcereadermock.cpp
    sourcereadermock.h
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

set(QTC_LIBRARIES
    -L${QTC_BUILD_DIR}/lib/qtcreator
    -L${QTC_BUILD_DIR}/lib/qtcreator/plugins
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

set_tests_properties(${TEST_NAME} PROPERTIES
    ENVIRONMENT "LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${QTC_BUILD_DIR}/lib/qtcreator:${QTC_BUILD_DIR}/lib/qtcreator/plugins"
)
