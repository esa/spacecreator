set(LIB_NAME libmsceditor)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    asn1fileview.cpp
    asn1fileview.h
    asn1fileview.ui
    documentitemmodel.cpp
    documentitemmodel.h
    documenttreeview.cpp
    documenttreeview.h
    mainmodel.cpp
    mainmodel.h
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    mscplugin.cpp
    mscplugin.h
    remotecontrolhandler.cpp
    remotecontrolhandler.h
    remotecontrolwebserver.cpp
    remotecontrolwebserver.h
    settings/appoptions.cpp
    settings/appoptions.h
)

target_include_directories(${LIB_NAME}
    PUBLIC .
)

target_link_libraries(${LIB_NAME}
    shared
    msclibrary
    mscwidgets
    asn1editor
    Qt5::Core
    Qt5::Gui
    Qt5::Network
    Qt5::Widgets
    Qt5::WebSockets
)
