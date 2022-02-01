set(TEST_NAME tsti_sedsplugin)
project(${TEST_NAME})

add_executable(${TEST_NAME}
    tsti_sedsplugin.cpp
    sedspluginwindow.cpp
    sedspluginwindow.h
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME} PUBLIC
    -L"${QTC_LIB_DIR}"
    -L"${QTC_PLUGINS_DIR}"
    commontestlib
    SedsPlugin
    ${QT_CORE}
    ${QT_TEST}
)

# configure_file(../../../../../build-spacecreator/lib/qtcreator/plugins/libSedsPlugin.so libSedsPlugin.so COPYONLY)
