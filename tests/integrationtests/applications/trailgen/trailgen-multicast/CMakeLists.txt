set(TEST_NAME tsti_trailgen_multicast)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
	PRIVATE    ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
	PRIVATE    ${CMAKE_SOURCE_DIR}/tests)
target_link_libraries(${TEST_NAME}
    PRIVATE    Qt${QT_VERSION_MAJOR}::Core
               Qt${QT_VERSION_MAJOR}::Test
               CommonProjectWarnings)

configure_file(test_trailgen_multicast.sh test_trailgen_multicast.sh COPYONLY)

add_custom_target(${TEST_NAME}_copy_all_files ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
	DEPENDS ${TEST_NAME})

add_format_target(${TEST_NAME})
add_tidy_target(${TEST_NAME})
