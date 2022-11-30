set(TEST_NAME tsti_interfaceviewoptimizer)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_testXML(${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/tests)
target_link_libraries(${TEST_NAME}
    PRIVATE     Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test)
add_dependencies(${TEST_NAME} tmc)

add_format_target(${TEST_NAME})

configure_file(test_interface_view_optimizer.sh test_interface_view_optimizer.sh COPYONLY)
configure_file(test_interface_view_optimizer_standalone.sh test_interface_view_optimizer_standalone.sh COPYONLY)
configure_file(test_interface_view_optimizer_keep.sh test_interface_view_optimizer_keep.sh COPYONLY)
configure_file(test_interface_view_optimizer_ergo_agent.sh test_interface_view_optimizer_ergo_agent.sh COPYONLY)
configure_file(test_interface_view_optimizer_ergo_battery.sh test_interface_view_optimizer_ergo_battery.sh COPYONLY)
add_custom_target(${TEST_NAME}_copy_files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    DEPENDS ${TEST_NAME})
