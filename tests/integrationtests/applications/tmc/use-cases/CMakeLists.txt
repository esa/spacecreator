set(TEST_NAME tst_tmc_usecases)
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

add_format_target(${TEST_NAME})

configure_file(test_dekker.sh test_dekker.sh COPYONLY)
configure_file(test_dekkerRunningWithInputObserver.sh test_dekkerRunningWithInputObserver.sh COPYONLY)
configure_file(test_dekkerRunningWithOutputObserver.sh test_dekkerRunningWithOutputObserver.sh COPYONLY)
configure_file(test_dekkerRunningWithStopCondition.sh test_dekkerRunningWithStopCondition.sh COPYONLY)

configure_file(test_uart.sh test_uart.sh COPYONLY)
configure_file(test_uartRunningWithInputObserver.sh test_uartRunningWithInputObserver.sh COPYONLY)
configure_file(test_uartRunningWithOutputObserver.sh test_uartRunningWithOutputObserver.sh COPYONLY)
configure_file(test_uartRunningWithStopCondition.sh test_uartRunningWithStopCondition.sh COPYONLY)

add_custom_target(${TEST_NAME}_copy_files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    DEPENDS ${TEST_NAME})
