set(LIB_NAME libdveditor)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    baseitems/graphicsview.cpp
    baseitems/graphicsview.h
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
    dveditor.cpp
    dveditor.h
    dveditorcore.cpp
    dveditorcore.h
    dveditorresources.qrc
    dvitemmodel.cpp
    dvitemmodel.h
    templating/exportabledvobject.cpp
    templating/exportabledvobject.h
    templating/exportabledvnode.cpp
    templating/exportabledvnode.h
    templating/exportabledvpartition.cpp
    templating/exportabledvpartition.h
    templating/exportabledvconnection.cpp
    templating/exportabledvconnection.h
    templating/exportabledvdevice.cpp
    templating/exportabledvdevice.h
    dvexporter.cpp
    dvexporter.h
    dvnodegraphicsitem.cpp
    dvnodegraphicsitem.h
    dvpartitiongraphicsitem.cpp
    dvpartitiongraphicsitem.h
    dvdevicegraphicsitem.cpp
    dvdevicegraphicsitem.h
    dvconnectiongraphicsitem.cpp
    dvconnectiongraphicsitem.h
    commands/cmdnodeentitycreate.cpp
    commands/cmdnodeentitycreate.h
    commands/cmdpartitionentitycreate.cpp
    commands/cmdpartitionentitycreate.h
    commands/cmdconnectionentitycreate.cpp
    commands/cmdconnectionentitycreate.h
    commands/cmddeviceentitycreate.cpp
    commands/cmddeviceentitycreate.h
    commands/cmdentitiesremove.cpp
    commands/cmdentitiesremove.h
    dvcreatortool.cpp
    dvcreatortool.h
    graphicsitemhelpers.cpp
    graphicsitemhelpers.h
    dvvisualizationmodel.cpp
    dvvisualizationmodel.h
    commands/cmdfunctionbind.cpp
    commands/cmdfunctionbind.h
    abstractsystemchecks.cpp
    abstractsystemchecks.h
    commands/cmdfunctionunbind.cpp
    commands/cmdfunctionunbind.h
    dvpropertiesdialog.cpp
    dvpropertiesdialog.h
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
