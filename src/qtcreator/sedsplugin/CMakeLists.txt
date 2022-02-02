set(PLUGIN_NAME SedsPlugin)

set(SEDS_PLUGIN_SOURCES
    SedsPlugin.json.in
    sedsplugin.cpp
    sedsplugin.h
    messagemanager.cpp
    messagemanager.h
)

set(SEDS_QTC_LIBRARIES
    -L"${QTC_LIB_DIR}"
    -L"${QTC_PLUGINS_DIR}"
    Aggregation
    ExtensionSystem
    Conversion::Converter
    Conversion::Registry
    Conversion::SedsRegistrar
    Conversion::SdlRegistrar
    Conversion::IvRegistrar
    Conversion::Asn1Registrar
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
    PUBLIC_DEPENDS SpaceCreatorPlugin spacecreatorsystem libmsceditor libiveditor libdveditor ${QT_CORE} ${QT_GUI} ${QT_WIDGETS} ${SEDS_QTC_LIBRARIES} ${SEDS_QTC_PLUGINS} 
    SOURCES ${SEDS_PLUGIN_SOURCES}
)

target_include_directories(${PLUGIN_NAME}
    PUBLIC src
            ${QTC_SOURCE_DIR}/src
            ${QTC_SOURCE_DIR}/src/libs
            ${QTC_SOURCE_DIR}/src/libs/aggregation
            ${QTC_SOURCE_DIR}/src/libs/extensionsystem
            ${QTC_SOURCE_DIR}/src/libs/utils
            ${QTC_SOURCE_DIR}/src/plugins
            ${QTC_SOURCE_DIR}/src/plugins/coreplugin
            ${QTC_SOURCE_DIR}/src/plugins/projectexplorer
            ${QTC_SOURCE_DIR}/src/plugins/qtsupport
            ${QTC_SOURCE_DIR}/src/plugins/texteditor
)
