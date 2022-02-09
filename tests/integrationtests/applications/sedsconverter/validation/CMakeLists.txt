set(TEST_NAME tst_sedsconverter_validation)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/tests)
target_link_libraries(${TEST_NAME}
    PRIVATE     Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test)

add_format_target(${TEST_NAME})

configure_file(test_invalid_fails.sh test_invalid_fails.sh COPYONLY)
configure_file(test_missing_core_semantics_fails.sh test_missing_core_semantics_fails.sh COPYONLY)
configure_file(test_missing_extension_semantics_fails.sh test_missing_extension_semantics_fails.sh COPYONLY)
configure_file(test_valid_passes.sh test_valid_passes.sh COPYONLY)

configure_file(resources/invalid/input-seds.xml resources/invalid/input-seds.xml COPYONLY)
configure_file(resources/invalid/seds-core-semantics.xsd resources/invalid/seds-core-semantics.xsd COPYONLY)
configure_file(resources/invalid/seds-extension-semantics.xsd resources/invalid/seds-extension-semantics.xsd COPYONLY)
configure_file(resources/invalid/seds.xsd resources/invalid/seds.xsd COPYONLY)

configure_file(resources/valid/input-seds.xml resources/valid/input-seds.xml COPYONLY)
configure_file(resources/valid/seds-core-semantics.xsd resources/valid/seds-core-semantics.xsd COPYONLY)
configure_file(resources/valid/seds-extension-semantics.xsd resources/valid/seds-extension-semantics.xsd COPYONLY)
configure_file(resources/valid/seds.xsd resources/valid/seds.xsd COPYONLY)

configure_file(resources/missing-core-semantics/input-seds.xml resources/missing-core-semantics/input-seds.xml COPYONLY)
configure_file(resources/missing-core-semantics/seds-extension-semantics.xsd resources/missing-core-semantics/seds-extension-semantics.xsd COPYONLY)
configure_file(resources/missing-core-semantics/seds.xsd resources/missing-core-semantics/seds.xsd COPYONLY)

configure_file(resources/missing-extension-semantics/input-seds.xml resources/missing-extension-semantics/input-seds.xml COPYONLY)
configure_file(resources/missing-extension-semantics/seds-core-semantics.xsd resources/missing-extension-semantics/seds-core-semantics.xsd COPYONLY)
configure_file(resources/missing-extension-semantics/seds.xsd resources/missing-extension-semantics/seds.xsd COPYONLY)
