set(LIB_NAME libdveditor)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    baseitems/graphicsview.cpp
    baseitems/graphicsview.h
    dveditor.cpp
    dveditor.h
    dveditorcore.cpp
    dveditorcore.h
    dveditorresources.qrc
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    deploymentmodel.cpp
    deploymentmodel.h
)

target_link_libraries(${LIB_NAME}
    shared
    Qt5::Svg
    Qt5::Widgets
    Qt5::Xml
)

target_include_directories(${LIB_NAME} PUBLIC .)

source_group("Designer Files" REGULAR_EXPRESSION ".*.ui$")
source_group("Resource Files" REGULAR_EXPRESSION ".*.qrc$")
