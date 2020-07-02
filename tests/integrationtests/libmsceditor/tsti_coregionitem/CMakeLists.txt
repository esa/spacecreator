set(TEST_NAME tsti_coregionitem)
project(${TEST_NAME})

add_executable(${TEST_NAME}
    tsti_coregionitem.cpp
    ../common/chartviewtestbase.cpp
    ../common/chartviewtestbase.h
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME} PUBLIC
    commontestlib
    libmsceditor
    Qt5::Core
    Qt5::Test
)

target_include_directories(${TEST_NAME}
    PUBLIC ../common
)
