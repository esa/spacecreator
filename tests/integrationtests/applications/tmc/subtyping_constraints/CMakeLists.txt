set(TEST_NAME tst_tmc_subtyping_constraints)
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

configure_file(test_basic_absent.sh test_basic_absent.sh COPYONLY)
configure_file(test_basic_present.sh test_basic_present.sh COPYONLY)
configure_file(test_choice_absent.sh test_choice_absent.sh COPYONLY)
configure_file(test_constrained_range.sh test_constrained_range.sh COPYONLY)

add_custom_target(${TEST_NAME}_copy_files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    DEPENDS ${TEST_NAME})
