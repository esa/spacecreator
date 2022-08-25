set(PLUGIN_NAME DeviceGeneratorPlugin)

set(DG_PLUGIN_SOURCES
    DeviceGeneratorPlugin.json.in
    devicegeneratorplugin_global.h
    devicegeneratorpluginconstants.h
    devicegeneratorplugin.h
    devicegeneratorplugin.cpp
)

set(DG_QTC_LIBRARIES
    -L"${QTC_LIB_DIR}"
    -L"${QTC_PLUGINS_DIR}"
    Aggregation
    ExtensionSystem
    Utils
)

set(DG_QTC_PLUGINS
    Core
    ProjectExplorer
    QtSupport
    TextEditor
)

add_qtc_plugin(${PLUGIN_NAME}
    DEFINES DEVICEGENERATOR_LIBRARY
    DEPENDS SpaceCreatorPlugin
            spacecreatorsystem
            libmsceditor
            libiveditor
            libdveditor
            CommonTools
            ${QT_CORE}
            ${QT_GUI}
            ${QT_WIDGETS}
            ${DG_QTC_LIBRARIES}
            ${DG_QTC_PLUGINS}
    PLUGIN_DEPENDS ${DG_QTC_PLUGINS}
    SOURCES ${DG_PLUGIN_SOURCES}
)

target_include_directories(${PLUGIN_NAME}
    PRIVATE ${QTC_SOURCE_DIR}/src/libs
    PRIVATE ${QTC_SOURCE_DIR}/src/plugins
    PRIVATE ${QTC_SOURCE_DIR}/src/plugins/coreplugin
)

source_group("Designer Files" REGULAR_EXPRESSION ".*.ui$")
source_group("Resource Files" REGULAR_EXPRESSION ".*.qrc$")