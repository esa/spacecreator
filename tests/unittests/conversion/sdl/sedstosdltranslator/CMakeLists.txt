set(TEST_NAME tst_sedstosdltranslator)
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
    PRIVATE     Conversion::SedsToSdlTranslator                
                ConversionTests::Asn1ModelBuilder
                ConversionTests::SedsModelBuilder
                SDL::Model
                Qt5::Core
                Qt5::Test
                CommonProjectWarnings)

add_format_target(${TEST_NAME})
