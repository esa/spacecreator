set(LIB_NAME asn1editor)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    asn1_resources.qrc
    asn1editorconst.h
    asn1editor.cpp
    asn1editor.h
    asn1editor.ui
    asn1fileview.cpp
    asn1fileview.h
    asn1fileview.ui
    asn1itemdelegate.cpp
    asn1itemdelegate.h
    asn1itemmodel.cpp
    asn1itemmodel.h
    asn1treeview.cpp
    asn1treeview.h
)

target_include_directories(${LIB_NAME}
    PUBLIC .
)

target_link_libraries(${LIB_NAME}
    asn1library
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)

source_group("Designer Files" REGULAR_EXPRESSION ".*.ui$")
source_group("Resource Files" REGULAR_EXPRESSION ".*.qrc$")
