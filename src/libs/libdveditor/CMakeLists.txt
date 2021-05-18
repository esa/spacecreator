set(LIB_NAME libdveditor)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    baseitems/graphicsview.cpp
    baseitems/graphicsview.h
    dvappmodel.cpp
    dvappmodel.h
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
    templating/exportabledvprocessor.cpp
    templating/exportabledvprocessor.h
    templating/exportabledvpartition.cpp
    templating/exportabledvpartition.h
    templating/exportabledvbinding.cpp
    templating/exportabledvbinding.h
    templating/exportabledvbus.cpp
    templating/exportabledvbus.h
    templating/exportabledvconnection.cpp
    templating/exportabledvconnection.h
    templating/exportabledvdevice.cpp
    templating/exportabledvdevice.h
    dvexporter.cpp
    dvexporter.h
    dvnodegraphicsitem.cpp
    dvnodegraphicsitem.h
    dvprocessorgraphicsitem.cpp
    dvprocessorgraphicsitem.h
    dvpartitiongraphicsitem.cpp
    dvpartitiongraphicsitem.h
    dvdevicegraphicsitem.cpp
    dvdevicegraphicsitem.h
    dvbusgraphicsitem.cpp
    dvbusgraphicsitem.h
    dvconnectiongraphicsitem.cpp
    dvconnectiongraphicsitem.h
    dvbindinggraphicsitem.cpp
    dvbindinggraphicsitem.h
    dvinterfacegraphicsitem.cpp
    dvinterfacegraphicsitem.h
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
