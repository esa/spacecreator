set(TEST_NAME tst_tmc_multicast)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/tests)
target_link_libraries(${TEST_NAME}
    PRIVATE     Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test
                CommonProjectWarnings)

add_format_target(${TEST_NAME})

configure_file(test_sporadic_many_to_one.sh test_sporadic_many_to_one.sh COPYONLY)
configure_file(test_sporadic_one_to_many.sh test_sporadic_one_to_many.sh COPYONLY)
configure_file(test_protected_many_to_one.sh test_protected_many_to_one.sh COPYONLY)
configure_file(test_protected_one_to_many.sh test_protected_one_to_many.sh COPYONLY)
add_custom_target(${TEST_NAME}_copy_files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    DEPENDS ${TEST_NAME})
