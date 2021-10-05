set(TEST_NAME tst_sdlexporter)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs
    PRIVATE     ${CMAKE_SOURCE_DIR}/tests)
target_link_libraries(${TEST_NAME}
    PRIVATE     SdlExporter
                SdlModel
                Qt5::Core
                Qt5::Test
                CommonProjectWarnings)

add_format_target(${TEST_NAME})
