set(TEST_NAME tsti_simulinktoiv)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_testXML(${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(${TEST_NAME}
    PRIVATE     Conversion::Converter
                Conversion::Registry
                Conversion::IvRegistrar;
                Conversion::SimulinkRegistrar;
                Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test)

configure_file(resources/config.xml config.xml COPYONLY)

add_format_target(${TEST_NAME})

add_custom_target(${TEST_NAME}_copy_files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    DEPENDS ${TEST_NAME})
