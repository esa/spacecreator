set(PLUGIN_NAME MscPlugin)

set(MSC_PLUGIN_SOURCES
    MscPlugin.json.in
    mainwidget.cpp
    mainwidget.h
    msccontext.cpp
    msccontext.h
    msceditordata.cpp
    msceditordata.h
    msceditordocument.cpp
    msceditordocument.h
    msceditorfactory.cpp
    msceditorfactory.h
    msceditorstack.cpp
    msceditorstack.h
    mscpluginconstants.h
    mscpluginplugin.cpp
    mscpluginplugin.h
    mscplugin_global.h
    msctexteditor.cpp
    msctexteditor.h
)

set(MSC_QTC_LIBRARIES
    -L"${QTC_LIB_DIR}"
    -L"${QTC_PLUGINS_DIR}"
    Aggregation
    ExtensionSystem
    Utils
)
set(MSC_QTC_PLUGINS
    Core
    ProjectExplorer
    QtSupport
    TextEditor
)

add_qtc_plugin(${PLUGIN_NAME}
    DEFINES MSCPLUGIN_LIBRARY
    DEPENDS libmsceditor Qt5::Core Qt5::Gui Qt5::Widgets ${MSC_QTC_LIBRARIES} ${MSC_QTC_PLUGINS}
    PLUGIN_DEPENDS ${MSC_QTC_PLUGINS}
    SOURCES ${MSC_PLUGIN_SOURCES}
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
