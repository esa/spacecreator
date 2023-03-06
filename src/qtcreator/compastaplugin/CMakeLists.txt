set(PLUGIN_NAME CompastaPlugin)

set(COMPASTA_PLUGIN_SOURCES
    CompastaPlugin.json.in
    compastapluginconstants.h
    compastaplugin.cpp
    compastaplugin.h
    messagemanager.cpp
    messagemanager.h
    slimconstants.h
    slimsnippets.cpp
    slimsnippets.h
    tr.h
)

set(COMPASTA_QTC_LIBRARIES
    -L"${QTC_LIB_DIR}"
    -L"${QTC_PLUGINS_DIR}"
    Aggregation
    ExtensionSystem
    Utils
)
set(COMPASTA_QTC_PLUGINS
    Core
    ProjectExplorer
    QtSupport
    TextEditor
)

add_qtc_plugin(${PLUGIN_NAME}
    DEFINES COMPASTAPLUGIN_LIBRARY
    DEPENDS SpaceCreatorPlugin spacecreatorsystem ${QT_CORE} ${QT_GUI} ${QT_WIDGETS} ${COMPASTA_QTC_LIBRARIES} ${COMPASTA_QTC_PLUGINS}
    PLUGIN_DEPENDS ${COMPASTA_QTC_PLUGINS}
    SOURCES ${COMPASTA_PLUGIN_SOURCES}
)

target_include_directories(${PLUGIN_NAME}
    PRIVATE src
    PRIVATE ${QTC_SOURCE_DIR}/src
    PRIVATE ${QTC_SOURCE_DIR}/src/libs
    PRIVATE ${QTC_SOURCE_DIR}/src/libs/aggregation
    PRIVATE ${QTC_SOURCE_DIR}/src/libs/extensionsystem
    PRIVATE ${QTC_SOURCE_DIR}/src/libs/utils
    PRIVATE ${QTC_SOURCE_DIR}/src/plugins
    PRIVATE ${QTC_SOURCE_DIR}/src/plugins/coreplugin
    PRIVATE ${QTC_SOURCE_DIR}/src/plugins/projectexplorer
    PRIVATE ${QTC_SOURCE_DIR}/src/plugins/qtsupport
    PRIVATE ${QTC_SOURCE_DIR}/src/plugins/texteditor
)

install(FILES generic-highlighter/syntax/slim.xml DESTINATION ${IDE_DATA_PATH}/generic-highlighter)
install(DIRECTORY snippets DESTINATION ${IDE_DATA_PATH})
