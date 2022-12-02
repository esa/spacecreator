# Functions supporting tests
#

# This is to be used instead of cmake built in "add_test()"
# It adds command parameters for the test output
#
function(add_testXML TEST_NAME)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME} -o ${CMAKE_BINARY_DIR}/${TEST_NAME}.xml,xunitxml)
endfunction()


# Create one test that is called "TEST_NAME" and has a test source TEST_NAME.cpp
# LIBRARY is the LIBRARY (a list is possible as well) the test depends on
# Optionally you can add a list of aditional source files
#
# Example: addQtTest(tst_ivobject ivcore "extra01.xml;extra02.xml;extra03.xml")
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

    add_testXML(${TEST_NAME})

    if(OPTIONS_NO_DEPRECATED)
        target_compile_options(${TEST_NAME} PRIVATE -Wno-deprecated -Wno-deprecated-declarations)
    endif()
endfunction()
