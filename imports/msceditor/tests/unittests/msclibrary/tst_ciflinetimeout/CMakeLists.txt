set(TEST_NAME tst_ciflinetimeout)

add_executable(${TEST_NAME}
    tst_ciflinetimeout.cpp
    ../cifcommon/ciflinepointstestbase.h
    ../cifcommon/ciflinepointstestbase.cpp
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME} -xunitxml -o ${CMAKE_BINARY_DIR}/${TEST_NAME}_result.xml)

target_link_libraries(${TEST_NAME} PUBLIC
    msclibrary
    Qt5::Core
    Qt5::Test
)

target_include_directories(${TEST_NAME} PUBLIC ../cifcommon)
