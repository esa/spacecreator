set(TEST_NAME tst_indenter)
project(${TEST_NAME})

add_executable(${TEST_NAME}
    indenter_tests.cpp
    indenter_tests.h
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

set(QTC_LIBRARIES
    -L${QTC_BUILD_DIR}/lib/qtcreator
    -L${QTC_BUILD_DIR}/lib/qtcreator/plugins
    Aggregation
    ExtensionSystem
    Utils
)
set(QTC_PLUGINS
    Core
    TextEditor
)

target_link_libraries(${TEST_NAME} PUBLIC
    Asn1Acn
    ${QTC_LIBRARIES}
    ${QTC_PLUGINS}
    Qt5::Core
    Qt5::Gui
    Qt5::Test
)

set_tests_properties(${TEST_NAME} PROPERTIES
    ENVIRONMENT "LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${QTC_BUILD_DIR}/lib/qtcreator:${QTC_BUILD_DIR}/lib/qtcreator/plugins"
)
