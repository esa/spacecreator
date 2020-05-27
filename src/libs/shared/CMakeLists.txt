set(LIB_NAME shared)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    reports/bugreportdialog.cpp
    reports/bugreportdialog.h
    reports/bugreportdialog.ui
    reports/bugreporthandler.cpp
    reports/bugreporthandler.h
    ui/drawrectinfo.cpp
    ui/drawrectinfo.h
    ui/graphicsviewbase.cpp
    ui/graphicsviewbase.h
    ui/grippoint.cpp
    ui/grippoint.h
    ui/grippointshandler.cpp
    ui/grippointshandler.h
    ui/highlightrectitem.cpp
    ui/highlightrectitem.h
    ui/interactiveobjectbase.cpp
    ui/interactiveobjectbase.h
    animation.cpp
    animation.h
    commandlineparser.cpp
    commandlineparser.h
    common.cpp
    common.h
    delayedsignal.cpp
    delayedsignal.h
    graphicsviewutils.cpp
    graphicsviewutils.h
    plugin.cpp
    plugin.h
    settingsappoption.cpp
    settingsappoption.h
    settingsmanager.cpp
    settingsmanager.h
    sharedlibrary.cpp
    sharedlibrary.h

    sharedresources/sharedresources.qrc
)

target_include_directories(${LIB_NAME} PUBLIC .)

target_link_libraries(${LIB_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Network
    Qt5::Concurrent
)
