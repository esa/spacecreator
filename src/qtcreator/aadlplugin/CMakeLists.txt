set(PLUGIN_NAME AadlPlugin)

set(MSC_PLUGIN_SOURCES
    AadlPlugin.json.in
    aadlcontext.cpp
    aadlcontext.h
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
    aadlplugin.cpp
    aadlplugin.h
    aadlplugin_global.h
    aadlpluginconstants.h
    aadltexteditor.cpp
    aadltexteditor.h
)

set(AADL_QTC_LIBRARIES
    -L"${QTC_LIB_DIR}"
    -L"${QTC_PLUGINS_DIR}"
    Aggregation
    ExtensionSystem
    Utils
)
set(AADL_QTC_PLUGINS
    Core
    ProjectExplorer
    QtSupport
    TextEditor
)

add_qtc_plugin(${PLUGIN_NAME}
    DEFINES AADLPLUGIN_LIBRARY
    DEPENDS libiveditor Qt5::Core Qt5::Gui Qt5::Widgets ${AADL_QTC_LIBRARIES} ${AADL_QTC_PLUGINS}
    PLUGIN_DEPENDS ${AADL_QTC_PLUGINS}
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
