#project(visitors)

#add_executable(VisitorsTests)
#target_sources(VisitorsTests PRIVATE failing_case.cpp)

#addQtTest(asn1nodereconstructingvisitor_tests asn1library)

set(TEST_NAME asn1nodereconstructingvisitor_tests)
add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
   PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
   PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(${TEST_NAME}
   PRIVATE     Conversion::Common
               asn1library
               Qt5::Core
               Qt5::Test)

add_format_target(${TEST_NAME})
