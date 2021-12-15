set(TEST_NAME tst_sedstoivtranslator)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs
                ${CMAKE_SOURCE_DIR}/tests
                ${CMAKE_SOURCE_DIR}/tests/unittests/conversion/common)
target_link_libraries(${TEST_NAME}
    PRIVATE     Conversion::SedsToIvTranslator
                Conversion::SedsToAsn1Translator
                ConversionTests::Asn1ModelBuilder
                ConversionTests::SedsModelBuilder
                Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test
                CommonProjectWarnings)

configure_file(resources/config.xml config.xml COPYONLY)

add_format_target(${TEST_NAME})
