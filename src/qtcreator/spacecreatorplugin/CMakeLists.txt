set(PLUGIN_NAME SpaceCreatorPlugin)

set(SC_PLUGIN_SOURCES
    SpaceCreatorPlugin.json.in
    dv/dveditordata.cpp
    dv/dveditordata.h
    dv/dveditordocument.cpp
    dv/dveditordocument.h
    dv/dveditorfactory.cpp
    dv/dveditorfactory.h
    dv/dvmainwidget.cpp
    dv/dvmainwidget.h
    dv/dvqtceditor.cpp
    dv/dvqtceditor.h
    iv/iveditordata.cpp
    iv/iveditordata.h
    iv/iveditordocument.cpp
    iv/iveditordocument.h
    iv/iveditorfactory.cpp
    iv/iveditorfactory.h
    iv/ivmainwidget.cpp
    iv/ivmainwidget.h
    iv/ivqtceditor.cpp
    iv/ivqtceditor.h
    msc/msccontext.cpp
    msc/msccontext.h
    msc/msceditordata.cpp
    msc/msceditordata.h
    msc/msceditordocument.cpp
    msc/msceditordocument.h
    msc/msceditorfactory.cpp
    msc/msceditorfactory.h
    msc/mscmainwidget.cpp
    msc/mscmainwidget.h
    msc/mscqtceditor.cpp
    msc/mscqtceditor.h
    options/asn1sccoption.cpp
    options/asn1sccoption.h
    options/asn1sccoptionwidget.cpp
    options/asn1sccoptionwidget.h
    options/asn1sccoptionwidget.ui
    options/coloroption.cpp
    options/coloroption.h
    options/dynamicactionoption.cpp
    options/dynamicactionoption.h
    options/hwlibraryoption.cpp
    options/hwlibraryoption.h
    options/propertyoptions.cpp
    options/propertyoptions.h
    options/ivlibraryoption.cpp
    options/ivlibraryoption.h
    spacecreatorplugin.cpp
    spacecreatorplugin.h
    spacecreatorplugin_global.h
    spacecreatorpluginconstants.h
    spacecreatorprojectimpl.cpp
    spacecreatorprojectimpl.h
    spacecreatorprojectmanager.cpp
    spacecreatorprojectmanager.h
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
    DEPENDS spacecreatorsystem libmsceditor libiveditor libdveditor ${QT_CORE} ${QT_GUI} ${QT_WIDGETS} ${SC_QTC_LIBRARIES} ${SC_QTC_PLUGINS}
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

source_group("Designer Files" REGULAR_EXPRESSION ".*.ui$")
source_group("Resource Files" REGULAR_EXPRESSION ".*.qrc$")
