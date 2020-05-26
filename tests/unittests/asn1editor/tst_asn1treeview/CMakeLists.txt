set(TEST_NAME tst_asn1treeview)

add_executable(${TEST_NAME}
    tst_asn1treeview.cpp
    DataView.xml
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME} PUBLIC
    asn1editor
    Qt5::Core
    Qt5::Test
)
