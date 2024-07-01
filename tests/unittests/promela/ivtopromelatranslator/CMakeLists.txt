set(TEST_NAME tst_ivtopromelatranslator)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(${TEST_NAME}
    PRIVATE     Conversion::Common
                Conversion::IvToPromelaTranslator
                CommonTools
                Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test)

configure_file(resources/parameterless.xml parameterless.xml COPYONLY)
configure_file(resources/interface_params.xml interface_params.xml COPYONLY)
configure_file(resources/function_types.xml function_types.xml COPYONLY)
configure_file(resources/proctype_priority.xml proctype_priority.xml COPYONLY)
configure_file(resources/output_observers.xml output_observers.xml COPYONLY)
configure_file(resources/unhandled_input_observers.xml unhandled_input_observers.xml COPYONLY)
configure_file(resources/synchronous_interfaces.xml synchronous_interfaces.xml COPYONLY)
configure_file(resources/channel_names.xml channel_names.xml COPYONLY)
configure_file(resources/env_calls_sync.xml env_calls_sync.xml COPYONLY)
configure_file(resources/dataview.asn dataview.asn COPYONLY)

add_format_target(${TEST_NAME})
