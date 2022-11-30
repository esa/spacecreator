set(TEST_NAME tsti_inputvectorlimit)
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

configure_file(test_input_vector_limit.sh test_input_vector_limit.sh COPYONLY)
configure_file(test_input_vector_limit_zero.sh test_input_vector_limit_zero.sh COPYONLY)
add_custom_target(${TEST_NAME}_copy_files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    DEPENDS ${TEST_NAME})
