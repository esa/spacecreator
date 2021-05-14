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
)

target_link_libraries(${LIB_NAME}
    shared
    dvcore
    ${QT_SVG}
    ${QT_WIDGETS}
    ${QT_XML}
)

target_include_directories(${LIB_NAME} PUBLIC .)

source_group("Designer Files" REGULAR_EXPRESSION ".*.ui$")
source_group("Resource Files" REGULAR_EXPRESSION ".*.qrc$")
