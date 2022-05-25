set(PLUGIN_NAME FunctionTesterPlugin)

set(FT_PLUGIN_SOURCES
    FunctionTesterPlugin.json.in
    functiontesterplugin.cpp
    functiontesterplugin.h
    messagemanager.cpp
    messagemanager.h
)

set(FT_QTC_LIBRARIES
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

set(FT_QTC_PLUGINS
    Core
    ProjectExplorer
    QtSupport
    TextEditor
)

add_qtc_plugin(${PLUGIN_NAME}
    DEFINES FunctionTesterPlugin_LIBRARY
    DEPENDS SpaceCreatorPlugin
            spacecreatorsystem
            libmsceditor
            libiveditor
            libdveditor
            CommonTools
            ${QT_CORE}
            ${QT_GUI}
            ${QT_WIDGETS}
            ${FT_QTC_LIBRARIES}
            ${FT_QTC_PLUGINS}
    PLUGIN_DEPENDS ${FT_QTC_PLUGINS}
    SOURCES ${FT_PLUGIN_SOURCES}
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
