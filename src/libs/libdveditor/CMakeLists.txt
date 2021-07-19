set(LIB_NAME libdveditor)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    abstractsystemchecks.cpp
    abstractsystemchecks.h
    baseitems/graphicsview.cpp
    baseitems/graphicsview.h
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
    dvconnectiongraphicsitem.cpp
    dvconnectiongraphicsitem.h
    dvcreatortool.cpp
    dvcreatortool.h
    dvdevicegraphicsitem.cpp
    dvdevicegraphicsitem.h
    dveditor.cpp
    dveditor.h
    dveditorcore.cpp
    dveditorcore.h
    dveditorresources.qrc
    dvexporter.cpp
    dvexporter.h
    dvitemmodel.cpp
    dvitemmodel.h
    dvnodegraphicsitem.cpp
    dvnodegraphicsitem.h
    dvpartitiongraphicsitem.cpp
    dvpartitiongraphicsitem.h
    dvpropertiesdialog.cpp
    dvpropertiesdialog.h
    dvtreeviewmodel.cpp
    dvtreeviewmodel.h
    dvvisualizationmodel.cpp
    dvvisualizationmodel.h
    graphicsitemhelpers.cpp
    graphicsitemhelpers.h
    templating/exportabledvconnection.cpp
    templating/exportabledvconnection.h
    templating/exportabledvdevice.cpp
    templating/exportabledvdevice.h
    templating/exportabledvnode.cpp
    templating/exportabledvnode.h
    templating/exportabledvobject.cpp
    templating/exportabledvobject.h
    templating/exportabledvpartition.cpp
    templating/exportabledvpartition.h
)

target_link_libraries(${LIB_NAME}
    dvcore
    templating
    shared
    ${QT_SVG}
    ${QT_WIDGETS}
    ${QT_XML}
)

target_include_directories(${LIB_NAME} PUBLIC .)

source_group("Designer Files" REGULAR_EXPRESSION ".*.ui$")
source_group("Resource Files" REGULAR_EXPRESSION ".*.qrc$")
