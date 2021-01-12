set(PLUGIN_NAME SpaceCreatorPlugin)

set(SC_PLUGIN_SOURCES
    SpaceCreatorPlugin.json.in
    aadleditordata.cpp
    aadleditordata.h
    aadleditordocument.cpp
    aadleditordocument.h
    aadleditorfactory.cpp
    aadleditorfactory.h
    aadleditorstack.cpp
    aadleditorstack.h
    aadlmainwidget.cpp
    aadlmainwidget.h
    aadlmodelstorage.cpp
    aadlmodelstorage.h
    aadltexteditor.cpp
    aadltexteditor.h
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
    mscmainwidget.cpp
    mscmainwidget.h
    mscmodelstorage.cpp
    mscmodelstorage.h
    mscqtceditor.cpp
    mscqtceditor.h
    mscsystemchecks.cpp
    mscsystemchecks.h
    spacecreatorplugin.cpp
    spacecreatorplugin.h
    spacecreatorplugin_global.h
    spacecreatorpluginconstants.h
    deploymentmodelstorage.cpp
    deploymentmodelstorage.h
    deploymenteditorfactory.cpp
    deploymenteditorfactory.h
    deploymenteditordata.cpp
    deploymenteditordata.h
)

set(SC_QTC_LIBRARIES
    -L"${QTC_LIB_DIR}"
    -L"${QTC_PLUGINS_DIR}"
    Aggregation
    ExtensionSystem
    Utils
)
set(SC_QTC_PLUGINS
    Core
    ProjectExplorer
    QtSupport
    TextEditor
)

add_qtc_plugin(${PLUGIN_NAME}
    DEFINES SCPLUGIN_LIBRARY
    DEPENDS libmsceditor libiveditor libdveditor Qt5::Core Qt5::Gui Qt5::Widgets ${SC_QTC_LIBRARIES} ${SC_QTC_PLUGINS}
    PLUGIN_DEPENDS ${SC_QTC_PLUGINS}
    SOURCES ${SC_PLUGIN_SOURCES}
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
