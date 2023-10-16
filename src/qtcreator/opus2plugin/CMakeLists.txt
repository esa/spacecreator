set(PLUGIN_NAME Opus2Plugin)

set(OPUS2_PLUGIN_SOURCES
    Opus2Plugin.json.in
    opus2pluginconstants.h
    opus2plugin.h
    opus2plugin.cpp
)

set(OPUS2_QTC_LIBRARIES
    -L"${QTC_LIB_DIR}"
    -L"${QTC_PLUGINS_DIR}"
    Aggregation
    ExtensionSystem
    Utils
)

set(OPUS2_QTC_PLUGINS
    Core
    ProjectExplorer
    QtSupport
    TextEditor
)

add_qtc_plugin(${PLUGIN_NAME}
    DEFINES DEVICEGENERATOR_LIBRARY
    DEPENDS SpaceCreatorPlugin
            spacecreatorsystem
            IvMerger
            libmsceditor
            libiveditor
            libdveditor
            CommonTools
            ${QT_CORE}
            ${QT_GUI}
            ${QT_WIDGETS}
            ${OPUS2_QTC_LIBRARIES}
            ${OPUS2_QTC_PLUGINS}
    PLUGIN_DEPENDS ${OPUS2_QTC_PLUGINS}
    SOURCES ${OPUS2_PLUGIN_SOURCES}
)

target_include_directories(${PLUGIN_NAME}
    PRIVATE src
    PRIVATE ${QTC_SOURCE_DIR}/src/libs
    PRIVATE ${QTC_SOURCE_DIR}/src/plugins
    PRIVATE ${QTC_SOURCE_DIR}/src/plugins/coreplugin
    PRIVATE ${QTC_SOURCE_DIR}/src/plugins/projectexplorer
)