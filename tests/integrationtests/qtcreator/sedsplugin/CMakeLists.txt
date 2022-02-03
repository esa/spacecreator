set(TEST_NAME tsti_sedsplugin)
project(${TEST_NAME})

add_executable(${TEST_NAME}
    tsti_sedsplugin.cpp
    sedspluginwindow.cpp
    sedspluginwindow.h
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

# TODO: this should be private linking
target_link_libraries(${TEST_NAME} PUBLIC
    -L"${QTC_LIB_DIR}"
    -L"${QTC_PLUGINS_DIR}"
    -L"/usr/lib/x86_64-linux-gnu/qtcreator/plugins"
    commontestlib
    SedsPlugin
    ${QT_CORE}
    ${QT_TEST}
)

file(COPY resources/testProject DESTINATION resources/)
file(COPY resources/input-eds DESTINATION resources/)
file(MAKE_DIRECTORY output/)
