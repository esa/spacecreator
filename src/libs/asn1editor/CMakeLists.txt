set(LIB_NAME asn1editor)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    asn1editor.h
    asn1itemdelegate.h
    asn1treeview.h
    asn1xmlparser.h
    asn1valueparser.h
    asn1itemmodel.h
    asn1const.h
    asn1editor.cpp
    asn1itemdelegate.cpp
    asn1treeview.cpp
    asn1xmlparser.cpp
    asn1valueparser.cpp
    asn1itemmodel.cpp
    asn1editor.ui
    asn1_resources.qrc
)

target_include_directories(${LIB_NAME}
    PUBLIC .
)

target_link_libraries(${LIB_NAME}
    asn1library
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Xml
)

source_group("Designer Files" REGULAR_EXPRESSION ".*.ui$")
source_group("Resource Files" REGULAR_EXPRESSION ".*.qrc$")
