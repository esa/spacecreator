set(LIB_NAME shared)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    colors/colorhandler.cpp
    colors/colorhandler.h
    colors/colorhandlereditor.cpp
    colors/colorhandlereditor.h
    colors/colorhandlereditor.ui
    colors/colormanager.cpp
    colors/colormanager.h
    colors/colormanagerdialog.cpp
    colors/colormanagerdialog.h
    colors/colormanagerdialog.ui
    colors/colorselectorbutton.cpp
    colors/colorselectorbutton.h
    colors/colorsettingswidget.cpp
    colors/colorsettingswidget.h
    colors/colorsettingswidget.ui
    reports/bugreportdialog.cpp
    reports/bugreportdialog.h
    reports/bugreportdialog.ui
    reports/bugreporthandler.cpp
    reports/bugreporthandler.h
    ui/drawrectinfo.cpp
    ui/drawrectinfo.h
    ui/graphicsscenebase.cpp
    ui/graphicsscenebase.h
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
    ui/textitem.cpp
    ui/textitem.h
    actionsbar.cpp
    actionsbar.h
    animation.cpp
    animation.h
    commandlineparser.cpp
    commandlineparser.h
    common.cpp
    common.h
    delayedsignal.cpp
    delayedsignal.h
    editorcore.cpp
    editorcore.h
    geometry.cpp
    geometry.h
    graphicsviewutils.cpp
    graphicsviewutils.h
    minimap.h
    minimap.cpp
    settingsmanager.cpp
    settingsmanager.h
    sharedlibrary.cpp
    sharedlibrary.h
    sharedresources/colors/default_colors.json
    sharedresources/sharedresources.qrc
    undocommand.cpp
    undocommand.h
    commandsstackbase.cpp
    commandsstackbase.h
)

target_include_directories(${LIB_NAME} PUBLIC .)

target_link_libraries(${LIB_NAME}
    ${QT_CORE}
    ${QT_GUI}
    ${QT_WIDGETS}
    ${QT_NETWORK}
    ${QT_CONCURRENT}
)

source_group("Designer Files" REGULAR_EXPRESSION ".*.ui$")
source_group("Resource Files" REGULAR_EXPRESSION ".*.qrc$")
