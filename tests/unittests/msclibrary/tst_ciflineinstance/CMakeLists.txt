set(TEST_NAME tst_ciflineinstance)
project(${TEST_NAME})

add_executable(${TEST_NAME}
    tst_ciflineinstance.cpp
    ../cifcommon/ciflinepointstestbase.h
    ../cifcommon/ciflinepointstestbase.cpp
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME} PUBLIC
    msclibrary
    Qt5::Core
    Qt5::Test
)

target_include_directories(${TEST_NAME} PUBLIC ../cifcommon)
