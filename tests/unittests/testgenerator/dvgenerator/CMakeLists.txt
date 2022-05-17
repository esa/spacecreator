set(TEST_NAME tst_dvgenerator)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp
                dvtools.cpp
                dvtools.h
)

target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs
                ${CMAKE_SOURCE_DIR}/tests/unittests
)

target_link_libraries(${TEST_NAME}
    PRIVATE     CommonTools
                ivcore
                dvcore
                libiveditor
                libdveditor
                TestGenerator
                Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test
                CommonProjectWarnings
)

add_format_target(${TEST_NAME})

configure_file(resources/deploymentview-linux-x86.dv.xml.out resources/deploymentview-linux-x86.dv.xml COPYONLY)
configure_file(resources/deploymentview-samv71.dv.xml.out resources/deploymentview-samv71.dv.xml COPYONLY)
configure_file(resources/Board1.xml "$ENV{HOME}/.local/share/${TEST_NAME}/HWlibrary/Board1.xml" COPYONLY)
configure_file(../../../../src/libs/libdveditor/templating/xml_templates/deploymentview.tmplt deploymentmentview.tmplt COPYONLY)
