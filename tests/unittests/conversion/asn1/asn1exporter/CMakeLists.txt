set(TEST_NAME tst_asn1exporter)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     tst_acnnodereconstructingvisitor.cpp
                tst_asn1nodereconstructingvisitor.cpp
                main.cpp
                tst_acnnodereconstructingvisitor.h
                tst_asn1nodereconstructingvisitor.h)
target_include_directories(${TEST_NAME}
   PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(${TEST_NAME}
   PRIVATE     Conversion::Common
               Conversion::Asn1Exporter
               Qt5::Core
               Qt5::Test
               CommonProjectWarnings)

add_format_target(${TEST_NAME})
add_tidy_target(${TEST_NAME})
