set(TEST_NAME tst_spintrailimporter)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
	PRIVATE ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
	PRIVATE ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(${TEST_NAME}
	PRIVATE Conversion::Common
         	SpinTrail::Importer
            Qt${QT_VERSION_MAJOR}::Core
            Qt${QT_VERSION_MAJOR}::Test)

configure_file(resources/scenario.spt scenario.spt COPYONLY)

add_format_target(${TEST_NAME})
