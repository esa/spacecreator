set(LIB_NAME libdveditor)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    abstractsystemchecks.cpp
    abstractsystemchecks.h
    commands/cmdconnectionentitycreate.cpp
    commands/cmdconnectionentitycreate.h
    commands/cmddeviceentitycreate.cpp
    commands/cmddeviceentitycreate.h
    commands/cmdentitiesremove.cpp
    commands/cmdentitiesremove.h
    commands/cmdfunctionbind.cpp
    commands/cmdfunctionbind.h
    commands/cmdfunctionunbind.cpp
    commands/cmdfunctionunbind.h
    commands/cmdmessagenetitycreate.cpp
    commands/cmdmessagenetitycreate.h
    commands/cmdnodeentitycreate.cpp
    commands/cmdnodeentitycreate.h
    commands/cmdpartitionentitycreate.cpp
    commands/cmdpartitionentitycreate.h
    config/dvhwlibrarydialog.cpp
    config/dvhwlibrarydialog.h
    config/dvhwlibrarydialog.ui
    config/dvhwlibrarywidget.cpp
    config/dvhwlibrarywidget.h
    config/dvhwlibrarywidget.ui
    dvappmodel.cpp
    dvappmodel.h
    dvappwidget.cpp
    dvappwidget.h
    dvappwidget.ui
    dvcreatortool.cpp
    dvcreatortool.h
    dveditor.cpp
    dveditor.h
    dveditorcore.cpp
    dveditorcore.h
    dveditorresources.qrc
    dvexporter.cpp
    dvexporter.h
    dvtreeviewmodel.cpp
    dvtreeviewmodel.h
    dvvisualizationmodel.cpp
    dvvisualizationmodel.h
    itemeditor/dvconnectiongraphicsitem.cpp
    itemeditor/dvconnectiongraphicsitem.h
    itemeditor/dvdevicegraphicsitem.cpp
    itemeditor/dvdevicegraphicsitem.h
    itemeditor/dvitemmodel.cpp
    itemeditor/dvitemmodel.h
    itemeditor/dvnodegraphicsitem.cpp
    itemeditor/dvnodegraphicsitem.h
    itemeditor/dvpartitiongraphicsitem.cpp
    itemeditor/dvpartitiongraphicsitem.h
    itemeditor/graphicsitemhelpers.cpp
    itemeditor/graphicsitemhelpers.h
    itemeditor/graphicsview.cpp
    itemeditor/graphicsview.h
    properties/dvmessagebindingsmodel.cpp
    properties/dvmessagebindingsmodel.h
    properties/dvmessagebindingswidget.cpp
    properties/dvmessagebindingswidget.h
    properties/dvpropertiesdialog.cpp
    properties/dvpropertiesdialog.h
    templating/exportabledvbus.cpp
    templating/exportabledvbus.h
    templating/exportabledvconnection.cpp
    templating/exportabledvconnection.h
    templating/exportabledvdevice.cpp
    templating/exportabledvdevice.h
    templating/exportabledvmessage.cpp
    templating/exportabledvmessage.h
    templating/exportabledvnode.cpp
    templating/exportabledvnode.h
    templating/exportabledvobject.cpp
    templating/exportabledvobject.h
    templating/exportabledvpartition.cpp
    templating/exportabledvpartition.h
    properties/dvattributedelegate.cpp
    properties/dvattributedelegate.h
)

target_link_libraries(${LIB_NAME}
    dvcore
    asn1editor
    templating
    shared
    ${QT_SVG}
    ${QT_WIDGETS}
    ${QT_XML}
)

target_include_directories(${LIB_NAME} PUBLIC .)

source_group("Designer Files" REGULAR_EXPRESSION ".*.ui$")
source_group("Resource Files" REGULAR_EXPRESSION ".*.qrc$")
