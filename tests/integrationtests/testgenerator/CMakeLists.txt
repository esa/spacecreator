set(TEST_NAME tst_testgenerator)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp
)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs
                ${CMAKE_SOURCE_DIR}/tests/integrationtests
)

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
    TestGenerator
    libiveditor
    SpaceCreatorPlugin
    spacecreatorsystem
    CommonTools
    CommonProjectWarnings
    ${QTC_LIBRARIES}
    ${QTC_PLUGINS}
    ${QT_TEST}
    ${QT_WIDGETS}
)

set_tests_properties(${TEST_NAME} PROPERTIES
    ENVIRONMENT "LD_LIBRARY_PATH=$<TARGET_FILE_DIR:TestGenerator>:${QTC_LIB_DIR}:${QTC_PLUGINS_DIR}:$LD_LIBRARY_PATH"
)

configure_file(resources/project/test.csv resources/project/test.csv COPYONLY)
configure_file(resources/project/interfaceview.xml resources/project/interfaceview.xml COPYONLY)
configure_file(resources/project/dataview-uniq.asn resources/project/dataview-uniq.asn COPYONLY)
configure_file(resources/project/config.xml resources/project/config.xml COPYONLY)
configure_file(resources/generated/testdriver.c resources/generated/testdriver.c COPYONLY)
configure_file(resources/generated/interfaceview.xml resources/generated/interfaceview.xml COPYONLY)
configure_file(resources/generated/deploymentview.dv.xml resources/generated/deploymentview.dv.xml COPYONLY)

add_format_target(${TEST_NAME})
