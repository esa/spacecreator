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

target_link_libraries(${TEST_NAME} PUBLIC
    TestGenerator
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Test
    libiveditor
    CommonTools
    CommonProjectWarnings
)

configure_file(resources/test.csv resources/test.csv COPYONLY)
configure_file(resources/interfaceview.xml resources/interfaceview.xml COPYONLY)
configure_file(resources/dataview-uniq.asn resources/dataview-uniq.asn COPYONLY)
configure_file(resources/config.xml resources/config.xml COPYONLY)
configure_file(resources/Board1.xml "$ENV{HOME}/.qttest/share/${TEST_NAME}/HWlibrary/Board1.xml" COPYONLY)
configure_file(resources/interfaceview.tmplt "$ENV{HOME}/.qttest/share/${TEST_NAME}/export_templates/aadl_xml/interfaceview.tmplt" COPYONLY)
configure_file(resources/deploymentview.tmplt "$ENV{HOME}/.qttest/share/${TEST_NAME}/export_templates/aadl_xml/deploymentview.tmplt" COPYONLY)
configure_file(resources/deploymentview.tmplt "$ENV{HOME}/.qttest/share/${TEST_NAME}/forbidden_names.txt" COPYONLY)

add_format_target(${TEST_NAME})
