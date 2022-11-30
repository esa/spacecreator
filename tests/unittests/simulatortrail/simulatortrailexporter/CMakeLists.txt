set(TEST_NAME tst_simulatortrailexporter)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_testXML(${TEST_NAME})


target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(${TEST_NAME}
    PRIVATE     Conversion::Common
	            Conversion::SpinTrailToSimulatorTrailTranslator
                SimulatorTrail::Exporter
                Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test)

configure_file(resources/expected_basic_types.sim expected_basic_types.sim COPYONLY)
configure_file(resources/expected_structures.sim expected_structures.sim COPYONLY)

add_format_target(${TEST_NAME})
