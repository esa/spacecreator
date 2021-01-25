# Functions supporting tests
#

# Create one test that is called "TEST_NAME" and has a test source TEST_NAME.cpp
# LIBRARY is the LIBRARY (a list is possible as well)the test depends on
# Optionally you can add a list of aditional source files
#
# Example: addQtTest(tst_aadlobj aadl "extra01.xml;extra02.xml;extra03.xml")
#
function(addQtTest TEST_NAME LIBRARY)
    set(EXTRA_SOURCES ${ARGV2})
    add_executable(${TEST_NAME}
        ${TEST_NAME}.cpp
        ${EXTRA_SOURCES}
    )

    target_link_libraries(${TEST_NAME} PUBLIC
        ${LIBRARY}
        ${QT_TEST}
    )

    target_compile_definitions(${TEST_NAME} PUBLIC EXAMPLES_DIR=\"${CMAKE_SOURCE_DIR}/examples/\")

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME} -xunitxml -o ${CMAKE_BINARY_DIR}/${TEST_NAME}.xml)
endfunction()
