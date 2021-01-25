set(TEST_NAME tsti_chartitem)
project(${TEST_NAME})

add_executable(${TEST_NAME}
    tsti_chartitem.cpp
    ../common/chartviewtestbase.cpp
    ../common/chartviewtestbase.h
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME} PUBLIC
    commontestlib
    libmsceditor
    ${QT_CORE}
    ${QT_TEST}
)

target_include_directories(${TEST_NAME}
    PUBLIC ../common
)
