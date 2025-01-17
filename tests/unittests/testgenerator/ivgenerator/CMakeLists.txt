set(TEST_NAME tst_ivgenerator)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp
)

target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs
                ${CMAKE_SOURCE_DIR}/tests/unittests
)

target_link_libraries(${TEST_NAME}
    PRIVATE     CommonTools
                ivcore
                dvcore
                TestGenerator
                Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test
                CommonProjectWarnings
)

add_format_target(${TEST_NAME})

configure_file(resources/interfaceview.xml resources/interfaceview.xml COPYONLY)
configure_file(resources/interfaceview_default_iface_name.xml resources/interfaceview_default_iface_name.xml COPYONLY)
configure_file(resources/config.xml resources/config.xml COPYONLY)
