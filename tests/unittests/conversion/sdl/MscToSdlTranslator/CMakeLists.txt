set(TEST_NAME tst_msctosdltranslator)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     main.cpp
                tst_tftable.cpp
                tst_tftable.h
                tst_mscparametervalueparser.cpp
                tst_mscparametervalueparser.h)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs
                ${CMAKE_SOURCE_DIR}/tests/unittests)
target_link_libraries(${TEST_NAME}
    PRIVATE     Conversion::MscToSdlTranslator
                Conversion::Common
                CommonTools
                Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test)

add_format_target(${TEST_NAME})

configure_file(resources/types.asn resources/types.asn COPYONLY)
