set(PLUGIN_NAME SedsPlugin)

set(SEDS_PLUGIN_SOURCES
    SedsPlugin.json.in
    sedsplugin.cpp
    sedsplugin.h
)

set(SEDS_QTC_LIBRARIES
    -L"${QTC_LIB_DIR}"
    -L"${QTC_PLUGINS_DIR}"
    Aggregation
    ExtensionSystem
    Asn1Importer
    Utils
)
set(SEDS_QTC_PLUGINS
    Core
    ProjectExplorer
    QtSupport
    TextEditor
)

add_qtc_plugin(${PLUGIN_NAME}
    DEFINES SEDSPLUGIN_LIBRARY
    DEPENDS SpaceCreatorPlugin spacecreatorsystem libmsceditor libiveditor libdveditor ${QT_CORE} ${QT_GUI} ${QT_WIDGETS} ${SEDS_QTC_LIBRARIES} ${SEDS_QTC_PLUGINS}
    PLUGIN_DEPENDS ${SEDS_QTC_PLUGINS}
    SOURCES ${SEDS_PLUGIN_SOURCES}
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
